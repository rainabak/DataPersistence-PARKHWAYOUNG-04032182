#include "SampleRepository.h"
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

static std::string toJson(const Sample& s)
{
    std::ostringstream oss;
    oss << "{"
        << "\"id\":"          << s.id                        << ","
        << "\"name\":\""      << escapeJson(s.name)          << "\","
        << "\"description\":\"" << escapeJson(s.description) << "\""
        << "}";
    return oss.str();
}

static std::string buildJson(const std::vector<Sample>& items, int nextId)
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

static Sample fromJson(const std::string& obj)
{
    Sample s{};
    s.id          = readInt(obj,    "id");
    s.name        = readString(obj, "name");
    s.description = readString(obj, "description");
    return s;
}

static void parseJson(const std::string& raw,
                      std::vector<Sample>& out,
                      int& nextId)
{
    out.clear();
    nextId = 1;
    if (raw.empty()) return;

    nextId = readInt(raw, "nextId");

    for (const auto& obj : splitItems(raw))
    {
        Sample s = fromJson(obj);
        if (s.id > 0)
            out.push_back(std::move(s));
    }

    // 파일의 nextId가 실제 최대 ID보다 작은 경우 보정
    for (const auto& s : out)
    {
        if (s.id >= nextId)
            nextId = s.id + 1;
    }
}

// ── SampleRepository ─────────────────────────────────────────────────────────

SampleRepository::SampleRepository(JsonFileStorage& storage)
    : m_storage(storage)
{
    const std::string raw = m_storage.load();
    parseJson(raw, m_samples, m_nextId);
}

void SampleRepository::add(const Sample& sample)
{
    Sample newSample  = sample;
    newSample.id      = m_nextId++;
    m_samples.push_back(newSample);
    m_storage.save(buildJson(m_samples, m_nextId));
}

std::vector<Sample> SampleRepository::findAll() const
{
    return m_samples;
}

Sample* SampleRepository::findById(int id)
{
    auto it = std::find_if(m_samples.begin(), m_samples.end(),
                           [id](const Sample& s) { return s.id == id; });
    return it != m_samples.end() ? &(*it) : nullptr;
}

bool SampleRepository::update(const Sample& sample)
{
    auto it = std::find_if(m_samples.begin(), m_samples.end(),
                           [&sample](const Sample& s) { return s.id == sample.id; });
    if (it == m_samples.end()) return false;

    *it = sample;
    m_storage.save(buildJson(m_samples, m_nextId));
    return true;
}

bool SampleRepository::remove(int id)
{
    auto it = std::find_if(m_samples.begin(), m_samples.end(),
                           [id](const Sample& s) { return s.id == id; });
    if (it == m_samples.end()) return false;

    m_samples.erase(it);
    m_storage.save(buildJson(m_samples, m_nextId));
    return true;
}
