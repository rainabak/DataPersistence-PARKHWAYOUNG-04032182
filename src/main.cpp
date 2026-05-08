#include "utils/ConsoleUtil.h"
#include "views/MainMenuView.h"
#include "views/SampleView.h"
#include "views/OrderView.h"
#include "views/ProductionLineView.h"
#include "views/MonitoringView.h"
#include "views/ShipmentView.h"
#include "controllers/MainController.h"
#include "controllers/SampleController.h"
#include "controllers/OrderController.h"
#include "controllers/ProductionLineController.h"
#include "controllers/MonitoringController.h"
#include "controllers/ShipmentController.h"

int main()
{
    MainMenuView        mainView;
    SampleView          sampleView;
    OrderView           orderView;
    ProductionLineView  productionLineView;
    MonitoringView      monitoringView;
    ShipmentView        shipmentView;

    SampleController        sampleCtrl(sampleView);
    OrderController         orderCtrl(orderView);
    ProductionLineController productionLineCtrl(productionLineView);
    MonitoringController    monitoringCtrl(monitoringView);
    ShipmentController      shipmentCtrl(shipmentView);

    MainController controller(mainView,
                              sampleCtrl,
                              orderCtrl,
                              productionLineCtrl,
                              monitoringCtrl,
                              shipmentCtrl);
    controller.run();

    return 0;
}
