#include "dynamics_automation.h"

int main()
{
    CSVReader reader("hardware.csv");
    for (CSVRow &row : reader)
    {
        std::string prov_task = row["Prov Task"].get<std::string>();
        if(!prov_task.empty()){
            std::string site_id = get_site_id_by_prov_task(prov_task, api_key);
            std::string asset_id = get_asset_id_by_serial(row["S/N"].get<std::string>(), api_key);
            if (!asset_id.empty()) //if serial number exists?
            {
                std::string serial_number = row["S/N"].get<std::string>();
                if(!serial_number.empty()){
                    std::string asset_tag = row["Asset Tag"].get<std::string>();
                    update_hardware_in_dynamics(asset_id, asset_tag, site_id);
                    update_hardware_dispostion(asset_id, "RENTAL");
                }
            }
            else
            {
                std::cout << "Failed to get asset ID.\n";
            }
        }
        
    }
    return 0;
}
