/**********************************
*    = Header File Inclusion =
**********************************/

#include <list>
#include <map>
#include <boost/lexical_cast.hpp>
#include <Wt/Chart/WCartesianChart>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WStandardItemModel>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WTimer>
#include <Wt/WWidget>
#include <statgrab.h>
#include "sysmon.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace boost;
using namespace Wt;
using namespace Wt::Chart;
using namespace SamsungDForumIr;


/**********************************
*    = Constants =
**********************************/

/**********************************
*    = Enumerations =
**********************************/

/**********************************
*    = Properties =
**********************************/

/**********************************
*    = Fields =
**********************************/

/**********************************
*    = Constructos =
**********************************/

SysMon::SysMon(CgiRoot *cgi) : BaseWidget(cgi)
{
    this->clear();
    this->addWidget(Layout());

    Init();
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

/**********************************
*    = Event Handlers =
**********************************/

void SysMon::RefreshSysResourceUsage()
{
    sg_cpu_percents *cpu_percent;
    sg_mem_stats *mem_stats;
    sg_swap_stats *swap_stats;
    sg_host_info *general_stats;
    sg_disk_io_stats *diskio_stats;
    int num_diskio_stats;
    sg_network_io_stats *network_stats;
    int num_network_stats;

    // In order to make it work, you need to call it twice
    sg_snapshot();
    sg_snapshot();

    // Set Machine Info
    if ((general_stats = sg_get_host_info()) != NULL) {
        m_txtMachineNodeName->setText("Machine node name&nbsp;:&nbsp;&nbsp;<strong>" +
                                      lexical_cast<string>(general_stats->hostname) +
                                      "</strong>");
        m_txtMachineUptime->setText("Machine uptime&nbsp;&nbsp;&nbsp;<strong>" +
                                    lexical_cast<string>((long long)general_stats->uptime) +
                                    " seconds</strong>");
    }

    // Shift and Fill CPU Cache & Model
    if ((cpu_percent = sg_get_cpu_percents()) != NULL) {
        map < ECPUTypes, float > cpuMap;
        cpuMap[ECPUTypes_User] = cpu_percent->user;
        cpuMap[ECPUTypes_Kernel] = cpu_percent->kernel;
        cpuMap[ECPUTypes_IOWait] =cpu_percent->iowait;
        cpuMap[ECPUTypes_Swap] = cpu_percent->swap;
        cpuMap[ECPUTypes_Nice] = cpu_percent->nice;
        cpuMap[ECPUTypes_Idle] = cpu_percent->idle;

        m_cacheCpuUsage.push_back(cpuMap);
        m_cacheCpuUsage.pop_front();

        size_t i = 0;
        for (std::list< std::map < ECPUTypes, float > >::const_iterator it =
             m_cacheCpuUsage.begin(); it != m_cacheCpuUsage.end(); ++it) {
            cpuMap = *it;
            m_modelCpuUsage->setData(i, 0, cpuMap[ECPUTypes_User]);
            m_modelCpuUsage->setData(i, 1, cpuMap[ECPUTypes_Kernel]);
            m_modelCpuUsage->setData(i, 2, cpuMap[ECPUTypes_IOWait]);
            m_modelCpuUsage->setData(i, 3, cpuMap[ECPUTypes_Swap]);
            m_modelCpuUsage->setData(i, 4, cpuMap[ECPUTypes_Nice]);
            m_modelCpuUsage->setData(i, 5, cpuMap[ECPUTypes_Idle]);
            m_modelCpuUsage->setData(i, 6, i);
            ++i;
        }
    }

    // Shift and Fill Mem Cache & Model
    if(((mem_stats = sg_get_mem_stats()) != NULL) && (swap_stats = sg_get_swap_stats()) != NULL) {
        map < EMemTypes, float > memMap;
        memMap[EMemTypes_Total] = 100.0;
        memMap[EMemTypes_Used] = 100.0 * mem_stats->used / mem_stats->total;
        memMap[EMemTypes_Cache] = 100.0 * mem_stats->cache / mem_stats->total;
        memMap[EMemTypes_Free] = 100.0 * mem_stats->free / mem_stats->total;
        memMap[EMemTypes_SwapTotal] = swap_stats->total != 0 ?
                    100.0 : 0.0;
        memMap[EMemTypes_SwapUsed] = swap_stats->total != 0 ?
                    (100.0 * swap_stats->used) / swap_stats->total : 0;
        memMap[EMemTypes_SwapFree] = swap_stats->total != 0 ?
                    100.0 * swap_stats->free / swap_stats->total : 0;
        memMap[EMemTypes_TotalVMUsage] = 100.0 -
                (((float)(mem_stats->total + swap_stats->total) /
                  (float)(mem_stats->free + swap_stats->free)));

        m_cacheMemUsage.push_back(memMap);
        m_cacheMemUsage.pop_front();

        size_t i = 0;
        for (std::list< std::map < EMemTypes, float > >::const_iterator it =
             m_cacheMemUsage.begin(); it != m_cacheMemUsage.end(); ++it) {
            memMap = *it;
            m_modelMemUsage->setData(i, 0, memMap[EMemTypes_Total]);
            m_modelMemUsage->setData(i, 1, memMap[EMemTypes_Used]);
            m_modelMemUsage->setData(i, 2, memMap[EMemTypes_Cache]);
            m_modelMemUsage->setData(i, 3, memMap[EMemTypes_Free]);
            m_modelMemUsage->setData(i, 4, memMap[EMemTypes_SwapTotal]);
            m_modelMemUsage->setData(i, 5, memMap[EMemTypes_SwapUsed]);
            m_modelMemUsage->setData(i, 6, memMap[EMemTypes_SwapFree]);
            m_modelMemUsage->setData(i, 7, memMap[EMemTypes_TotalVMUsage]);
            m_modelMemUsage->setData(i, 8, i);
            ++i;
        }
    }

    // Set Disk Info
    m_dvSysMonDiskInfo->clear();

    new WText("<h4>Disk Info</h4>", m_dvSysMonDiskInfo);

    long long diskTrafficTotalDiskRead = 0;
    long long diskTrafficTotalDiskWrite = 0;

    WTable *diskTable = new WTable(m_dvSysMonDiskInfo);
    diskTable->setStyleClass("tbl");

    diskTable->elementAt(0, 0)->addWidget(new WText(L"Name"));
    diskTable->elementAt(0, 1)->addWidget(new WText(L"Read"));
    diskTable->elementAt(0, 2)->addWidget(new WText(L"Write"));
    diskTable->elementAt(0, 3)->addWidget(new WText(L"Systime"));
    diskTable->elementAt(0, 0)->setStyleClass("tblHeader");
    diskTable->elementAt(0, 1)->setStyleClass("tblHeader");
    diskTable->elementAt(0, 2)->setStyleClass("tblHeader");
    diskTable->elementAt(0, 3)->setStyleClass("tblHeader");

    if ((diskio_stats = sg_get_disk_io_stats_diff(&num_diskio_stats)) != NULL) {
        for (int i = 0; i < num_diskio_stats; ++i) {
            diskTable->elementAt(i + 1, 0)->addWidget(new WText(lexical_cast<string>(diskio_stats->disk_name)));
            diskTable->elementAt(i + 1, 1)->addWidget(new WText(lexical_cast<string>(diskio_stats->read_bytes)));
            diskTable->elementAt(i + 1, 2)->addWidget(new WText(lexical_cast<string>(diskio_stats->write_bytes)));
            diskTable->elementAt(i + 1, 3)->addWidget(new WText(lexical_cast<string>((long)diskio_stats->systime)));

            diskTrafficTotalDiskRead += diskio_stats->read_bytes;
            diskTrafficTotalDiskWrite += diskio_stats->write_bytes;

            ++diskio_stats;
        }

        diskTable->elementAt(num_diskio_stats + 1, 0)->addWidget(new WText("Total Disk IO"));
        diskTable->elementAt(num_diskio_stats + 1, 1)->addWidget(new WText(lexical_cast<string>(diskTrafficTotalDiskRead)));
        diskTable->elementAt(num_diskio_stats + 1, 2)->addWidget(new WText(lexical_cast<string>(diskio_stats->write_bytes)));
        diskTable->elementAt(num_diskio_stats + 1, 3)->addWidget(new WText("-"));
        new WBreak(m_dvSysMonDiskInfo);
    }

    // Set Net Info
    m_dvSysMonNetInfo->clear();

    new WText("<h4>Network Interface Info</h4>", m_dvSysMonNetInfo);

    long long netTrafficTotalRx = 0;
    long long netTrafficTotalTx = 0;
    long long netTrafficTotalPacketsIn = 0;
    long long netTrafficTotalPacketsOut = 0;
    long long netTrafficTotalErrorsIn = 0;
    long long netTrafficTotalErrorsOut = 0;
    long long netTrafficTotalCollisions = 0;

    WTable *netTable = new WTable(m_dvSysMonNetInfo);
    netTable->setStyleClass("tbl");

    netTable->elementAt(0, 0)->addWidget(new WText(L"Name"));
    netTable->elementAt(0, 1)->addWidget(new WText(L"Rx"));
    netTable->elementAt(0, 2)->addWidget(new WText(L"Tx"));
    netTable->elementAt(0, 3)->addWidget(new WText(L"Packets In"));
    netTable->elementAt(0, 4)->addWidget(new WText(L"Packets Out"));
    netTable->elementAt(0, 5)->addWidget(new WText(L"Errors In"));
    netTable->elementAt(0, 6)->addWidget(new WText(L"Errors Out"));
    netTable->elementAt(0, 7)->addWidget(new WText(L"Collisions"));
    netTable->elementAt(0, 8)->addWidget(new WText(L"Systime"));
    netTable->elementAt(0, 0)->setStyleClass("tblHeader");
    netTable->elementAt(0, 1)->setStyleClass("tblHeader");
    netTable->elementAt(0, 2)->setStyleClass("tblHeader");
    netTable->elementAt(0, 3)->setStyleClass("tblHeader");
    netTable->elementAt(0, 4)->setStyleClass("tblHeader");
    netTable->elementAt(0, 5)->setStyleClass("tblHeader");
    netTable->elementAt(0, 6)->setStyleClass("tblHeader");
    netTable->elementAt(0, 7)->setStyleClass("tblHeader");
    netTable->elementAt(0, 8)->setStyleClass("tblHeader");

    if ((network_stats = sg_get_network_io_stats_diff(&num_network_stats)) != NULL) {
        for (int i = 0; i < num_network_stats; ++i) {
            netTable->elementAt(i + 1, 0)->addWidget(new WText(lexical_cast<string>(network_stats->interface_name)));
            netTable->elementAt(i + 1, 1)->addWidget(new WText(lexical_cast<string>(network_stats->rx)));
            netTable->elementAt(i + 1, 2)->addWidget(new WText(lexical_cast<string>(network_stats->tx)));
            netTable->elementAt(i + 1, 3)->addWidget(new WText(lexical_cast<string>(network_stats->ipackets)));
            netTable->elementAt(i + 1, 4)->addWidget(new WText(lexical_cast<string>(network_stats->opackets)));
            netTable->elementAt(i + 1, 5)->addWidget(new WText(lexical_cast<string>(network_stats->ierrors)));
            netTable->elementAt(i + 1, 6)->addWidget(new WText(lexical_cast<string>(network_stats->oerrors)));
            netTable->elementAt(i + 1, 7)->addWidget(new WText(lexical_cast<string>(network_stats->collisions)));
            netTable->elementAt(i + 1, 8)->addWidget(new WText(lexical_cast<string>((long)network_stats->systime)));

            netTrafficTotalRx += network_stats->rx;
            netTrafficTotalTx += network_stats->tx;
            netTrafficTotalPacketsIn += network_stats->ipackets;
            netTrafficTotalPacketsOut += network_stats->opackets;
            netTrafficTotalErrorsIn += network_stats->ierrors;
            netTrafficTotalErrorsOut += network_stats->oerrors;
            netTrafficTotalCollisions += network_stats->collisions;

            new WBreak(m_dvSysMonNetInfo);

            ++network_stats;
        }

        netTable->elementAt(num_network_stats + 1, 0)->addWidget(new WText("Total Network IO"));
        netTable->elementAt(num_network_stats + 1, 1)->addWidget(new WText(lexical_cast<string>(netTrafficTotalRx)));
        netTable->elementAt(num_network_stats + 1, 2)->addWidget(new WText(lexical_cast<string>(netTrafficTotalTx)));
        netTable->elementAt(num_network_stats + 1, 3)->addWidget(new WText(lexical_cast<string>(netTrafficTotalPacketsIn)));
        netTable->elementAt(num_network_stats + 1, 4)->addWidget(new WText(lexical_cast<string>(netTrafficTotalPacketsOut)));
        netTable->elementAt(num_network_stats + 1, 5)->addWidget(new WText(lexical_cast<string>(netTrafficTotalErrorsIn)));
        netTable->elementAt(num_network_stats + 1, 6)->addWidget(new WText(lexical_cast<string>(netTrafficTotalErrorsOut)));
        netTable->elementAt(num_network_stats + 1, 7)->addWidget(new WText(lexical_cast<string>(netTrafficTotalCollisions)));
        netTable->elementAt(num_network_stats + 1, 8)->addWidget(new WText("-"));

        new WBreak(m_dvSysMonNetInfo);
    }

    sg_shutdown();
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

void SysMon::Init()
{
    // Filling CPU Model
    map < ECPUTypes, float > cpuMap;
    cpuMap[ECPUTypes_User] = 0.0;
    cpuMap[ECPUTypes_Kernel] = 0.0;
    cpuMap[ECPUTypes_IOWait] = 0.0;
    cpuMap[ECPUTypes_Swap] = 0.0;
    cpuMap[ECPUTypes_Nice] = 0.0;
    cpuMap[ECPUTypes_Idle] = 0.0;

    for (size_t i = 0; i < CPU_SEC_MAX; ++i)
        m_cacheCpuUsage.push_back(cpuMap);


    // Filling Mem Model
    map < EMemTypes, float > memMap;
    memMap[EMemTypes_Total] = 0.0;
    memMap[EMemTypes_Used] = 0.0;
    memMap[EMemTypes_Cache] = 0.0;
    memMap[EMemTypes_Free] = 0.0;
    memMap[EMemTypes_SwapTotal] = 0.0;
    memMap[EMemTypes_SwapUsed] = 0.0;
    memMap[EMemTypes_SwapFree] = 0.0;
    memMap[EMemTypes_TotalVMUsage] = 0.0;

    for (size_t i = 0; i < MEM_SEC_MAX; ++i)
        m_cacheMemUsage.push_back(memMap);


    // Init and Start Timer
    Wt::WTimer *timer = new Wt::WTimer();
    timer->setInterval(1000);
    timer->timeout().connect(this, &SysMon::RefreshSysResourceUsage);
    timer->start();
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *SysMon::Layout()
{
    Div *root = new Div("SysMon");

    // Machine Info
    new WBreak(root);
    Div *dvSysMonMachineInfo = new Div(root, "dvSysMonMachineInfo");
    m_txtMachineNodeName = new WText(dvSysMonMachineInfo);
    new WBreak(dvSysMonMachineInfo);
    m_txtMachineUptime = new WText(dvSysMonMachineInfo);
    new WBreak(dvSysMonMachineInfo);
    new WBreak(root);


    // CPU Model
    m_modelCpuUsage = new WStandardItemModel(60, 7, this);
    m_modelCpuUsage->setHeaderData(0, WString("User CPU"));
    m_modelCpuUsage->setHeaderData(1, WString("Kernel CPU"));
    m_modelCpuUsage->setHeaderData(2, WString("IOWait CPU"));
    m_modelCpuUsage->setHeaderData(3, WString("Swap CPU"));
    m_modelCpuUsage->setHeaderData(4, WString("Nice CPU"));
    m_modelCpuUsage->setHeaderData(5, WString("Idle CPU"));
    m_modelCpuUsage->setHeaderData(6, WString("-"));

    // Mem Model
    m_modelMemUsage = new WStandardItemModel(60, 9, this);
    m_modelMemUsage->setHeaderData(0, WString("Total Memory"));
    m_modelMemUsage->setHeaderData(1, WString("Used Memory"));
    m_modelMemUsage->setHeaderData(2, WString("Cache Memory"));
    m_modelMemUsage->setHeaderData(3, WString("Free Memory"));
    m_modelMemUsage->setHeaderData(4, WString("Swap Total"));
    m_modelMemUsage->setHeaderData(5, WString("Swap Used"));
    m_modelMemUsage->setHeaderData(6, WString("Swap Free"));
    m_modelMemUsage->setHeaderData(7, WString("Total VM Usage"));
    m_modelMemUsage->setHeaderData(8, WString("-"));


    // CPU Graph
    WCartesianChart *chartCpuUsage = new WCartesianChart(root);
    chartCpuUsage->setBackground(gray);
    chartCpuUsage->setModel(m_modelCpuUsage);
    chartCpuUsage->setXSeriesColumn(6);
    chartCpuUsage->setLegendEnabled(true);
    chartCpuUsage->setLegendLocation(LegendOutside, Right, AlignMiddle);

    chartCpuUsage->setType(ScatterPlot);
    chartCpuUsage->axis(XAxis).setScale(LinearScale);
    chartCpuUsage->axis(YAxis).setScale(LinearScale);

    chartCpuUsage->setPlotAreaPadding(120, Right);
    chartCpuUsage->setPlotAreaPadding(30, Top | Bottom);

    for (size_t i = 0; i < 6; ++i) {
        WDataSeries s(i, CurveSeries);
        s.setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3));
        chartCpuUsage->addSeries(s);
    }

    chartCpuUsage->resize(600, 330);

    chartCpuUsage->setMargin(10, Top | Bottom);
    chartCpuUsage->setMargin(WLength::Auto, Left | Right);


    // Mem Graph
    WCartesianChart *chartMemUsage = new WCartesianChart(root);
    chartMemUsage->setBackground(gray);
    chartMemUsage->setModel(m_modelMemUsage);
    chartMemUsage->setXSeriesColumn(8);
    chartMemUsage->setLegendEnabled(true);
    chartMemUsage->setLegendLocation(LegendOutside, Right, AlignMiddle);

    chartMemUsage->setType(ScatterPlot);
    chartMemUsage->axis(XAxis).setScale(LinearScale);
    chartMemUsage->axis(YAxis).setScale(LinearScale);

    chartMemUsage->setPlotAreaPadding(120, Right);
    chartMemUsage->setPlotAreaPadding(30, Top | Bottom);

    for (size_t i = 0; i < 8; ++i) {
        WDataSeries s(i, CurveSeries);
        s.setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3));
        chartMemUsage->addSeries(s);
    }

    chartMemUsage->resize(600, 330);

    chartMemUsage->setMargin(10, Top | Bottom);
    chartMemUsage->setMargin(WLength::Auto, Left | Right);

    new WBreak(root);

    // Disk Info
    m_dvSysMonDiskInfo = new Div(root, "dvSysMonDiskInfo");

    // Net Info
    m_dvSysMonNetInfo = new Div(root, "dvSysMonNetInfo");

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/

