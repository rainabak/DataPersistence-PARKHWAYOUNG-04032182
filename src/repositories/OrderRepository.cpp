#include "OrderRepository.h"
#include <algorithm>
#include <sstream>

// ── JSON 직렬화/역직렬화 (파일 내부 전용) ────────────────────────────────────

static std::string escapeJson(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (char c : s)
    {
        if      (c == '"')  out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\t') out += "\\t";
        else if (c == '\r') out += "\\r";
        else                out += c;
    }
    return out;
}

static std::string readString(const std::string& json, const std::string& key)
{
    const std::string search = "\"" + key + "\":";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return "";

    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;

    std::string result;
    while (pos < json.size() && json[pos] != '"')
    {
        if (json[pos] == '\\' && pos + 1 < json.size())
        {
            pos++;
            if      (json[pos] == '"')  result += '"';
            else if (json[pos] == '\\') result += '\\';
            else if (json[pos] == 'n')  result += '\n';
            else if (json[pos] == 't')  result += '\t';
            else if (json[pos] == 'r')  result += '\r';
            else                        result += json[pos];
        }
        else
        {
            result += json[pos];
        }
        pos++;
    }
    return result;
}

static int readInt(const std::string& json, const std::string& key)
{
    const std::string search = "\"" + key + "\":";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return 0;

    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;

    std::string numStr;
    if (pos < json.size() && json[pos] == '-') { numStr += '-'; pos++; }
    while (pos < json.size() && std::isdigit(static_cast<unsigned char>(json[pos])))
        numStr += json[pos++];

    return numStr.empty() ? 0 : std::stoi(numStr);
}

static std::vector<std::string> splitItems(const std::string& json)
{
    std::vector<std::string> result;

    const std::string search = "\"items\":";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return result;

    pos += search.size();
    while (pos < json.size() && json[pos] != '[') pos++;
    if (pos >= json.size()) return result;
    pos++;

    int    depth = 0;
    size_t start = std::string::npos;
    bool   inStr = false;

    for (size_t i = pos; i < json.size(); i++)
    {
        const char c = json[i];

        if (c == '"' && (i == 0 || json[i - 1] != '\\'))
        {
            inStr = !inStr;
        }
        else if (!inStr)
        {
            if (c == '{')
            {
                if (depth == 0) start = i;
                depth++;
            }
            else if (c == '}')
            {
                depth--;
                if (depth == 0 && start != std::string::npos)
                {
                    result.push_back(json.substr(start, i - start + 1));
                    start = std::string::npos;
                }
            }
            else if (c == ']' && depth == 0)
            {
                break;
            }
        }
    }

    return result;
}

static std::string toJson(const Order& o)
{
    std::ostringstream oss;
    oss << "{"
        << "\"id\":"           << o.id                        << ","
        << "\"productName\":\"" << escapeJson(o.productName)  << "\","
        << "\"quantity\":"     << o.quantity                  << ","
        << "\"status\":\""     << escapeJson(o.status)        << "\""
        << "}";
    return oss.str();
}

static std::string buildJson(const std::vector<Order>& items, int nextId)
{
    std::ostringstream oss;
    oss << "{\"nextId\":" << nextId << ",\"items\":[";
    for (size_t i = 0; i < items.size(); i++)
    {
        if (i > 0) oss << ",";
        oss << toJson(items[i]);
    }
    oss << "]}";
    return oss.str();
}

static Order fromJson(const std::string& obj)
{
    Order o{};
    o.id          = readInt(obj,    "id");
    o.productName = readString(obj, "productName");
    o.quantity    = readInt(obj,    "quantity");
    o.status      = readString(obj, "status");
    return o;
}

static void parseJson(const std::string& raw,
                      std::vector<Order>& out,
                      int& nextId)
{
    out.clear();
    nextId = 1;
    if (raw.empty()) return;

    nextId = readInt(raw, "nextId");

    for (const auto& obj : splitItems(raw))
    {
        Order o = fromJson(obj);
        if (o.id > 0)
            out.push_back(std::move(o));
    }

    // 파일의 nextId가 실제 최대 ID보다 작은 경우 보정
    for (const auto& o : out)
    {
        if (o.id >= nextId)
            nextId = o.id + 1;
    }
}

// ── OrderRepository ───────────────────────────────────────────────────────────

OrderRepository::OrderRepository(JsonFileStorage& storage)
    : m_storage(storage)
{
    const std::string raw = m_storage.load();
    parseJson(raw, m_orders, m_nextId);
}

void OrderRepository::add(const Order& order)
{
    Order newOrder  = order;
    newOrder.id     = m_nextId++;
    m_orders.push_back(newOrder);
    m_storage.save(buildJson(m_orders, m_nextId));
}

std::vector<Order> OrderRepository::findAll() const
{
    return m_orders;
}

Order* OrderRepository::findById(int id)
{
    auto it = std::find_if(m_orders.begin(), m_orders.end(),
                           [id](const Order& o) { return o.id == id; });
    return it != m_orders.end() ? &(*it) : nullptr;
}

bool OrderRepository::update(const Order& order)
{
    auto it = std::find_if(m_orders.begin(), m_orders.end(),
                           [&order](const Order& o) { return o.id == order.id; });
    if (it == m_orders.end()) return false;

    *it = order;
    m_storage.save(buildJson(m_orders, m_nextId));
    return true;
}

bool OrderRepository::remove(int id)
{
    auto it = std::find_if(m_orders.begin(), m_orders.end(),
                           [id](const Order& o) { return o.id == id; });
    if (it == m_orders.end()) return false;

    m_orders.erase(it);
    m_storage.save(buildJson(m_orders, m_nextId));
    return true;
}
