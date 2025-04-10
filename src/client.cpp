//
// Created by ender on 25-4-10.
//

#include "client.hpp"
#include "dto/api_request_dto.hpp"
#include "dto/api_response_dto.hpp"

namespace imouse {
    imouse_node_client::imouse_node_client(const std::string &base_url) {
        this->obj_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        this->executor = create_curl_executor(base_url);
        this->api_client = imouse_api_client::createShared(this->executor, obj_mapper);
    }

    auto imouse_node_client::list_groups() const -> std::map<std::string, std::string> {
        std::map<std::string, std::string> devices;

        const auto req_dto = dto::list_groups_request_dto::createShared();
        req_dto->fun = "get_group_list";
        req_dto->msgid = 0;
        const auto res = this->api_client->list_groups(req_dto);
        const auto res_dto = res->readBodyToDto<oatpp::Object<dto::list_groups_response_dto> >(this->obj_mapper);
        this->assert_res_dto_status(res_dto);
        for (const auto &[gid,group_dto]: *res_dto->data) {
            devices.emplace(gid, group_dto->name);
        }
        return devices;
    }

    auto imouse_node_client::list_devices() const -> std::map<std::string, oatpp::Object<dto::device_dto> > {
        std::map<std::string, oatpp::Object<dto::device_dto> > devices = {};
        const auto req_dto = dto::non_data_request_dto::createShared();
        req_dto->fun = "get_device_list";
        req_dto->msgid = 0;
        const auto res = this->api_client->list_devices(req_dto);
        const auto res_dto = res->readBodyToDto<oatpp::Object<dto::list_devices_response_dto> >(this->obj_mapper);
        this->assert_res_dto_status(res_dto);
        for (const auto &[device_id, device_dto]: *res_dto->data) {
            devices.emplace(device_id, device_dto);
        }
        return devices;
    }

    auto imouse_node_client::emplace_frame_callback(const std::string &device_id,
                                                    std::function<void(std::vector<std::byte>)> cb) {
        this->frame_callbacks[device_id] = std::move(cb);
    }

    std::shared_ptr<oatpp::web::client::RequestExecutor> imouse_node_client::create_curl_executor(
        const std::string &base_url) {
        OATPP_LOGD("App", "Using oatpp-curl RequestExecutor.");
        return oatpp::curl::RequestExecutor::createShared(base_url, false);
    }

    imouse_device_client::imouse_device_client(const std::shared_ptr<imouse_node_client> &node_client,
                                               const std::string &device_id) : node_client(node_client),
                                                                               device_id_(device_id) {
        this->node_client->emplace_frame_callback(device_id, [this](auto x) {
            if (this->frame_callback.has_value()) {
                this->frame_callback.value()(std::move(x));
            }
        });
    }

    auto imouse_device_client::create(const std::shared_ptr<imouse_node_client> &node_client,
                                      const std::string &device_id) -> std::shared_ptr<imouse_device_client> {
        if (const auto devices = node_client->list_devices(); not devices.contains(device_id)) {
            throw std::runtime_error(std::format("device[id={}] not found", device_id));
        }
        return std::make_shared<imouse_device_client>(node_client, device_id);
    }


    auto imouse_device_client::device_id() const -> std::string {
        return device_id_;
    }

    auto imouse_device_client::start_streaming() const -> void {
        const auto req_dto = dto::start_streaming_request::createShared();
        req_dto->fun = "loop_device_screenshot";
        req_dto->msgid = 0;
        req_dto->data->isjpg = true;
        req_dto->data->time = 1;
        req_dto->data->deviceid = this->device_id_;
        const auto res = this->node_client->api_client->start_streaming(req_dto);
        const auto res_dto = res->readBodyToDto<oatpp::Object<dto::device_id_response_dto> >(
            this->node_client->obj_mapper);
        this->node_client->assert_res_dto_status(res_dto);
    }

    auto imouse_device_client::stop_streaming() const -> void {
        const auto req_dto = dto::stop_streaming_request::createShared();
        req_dto->fun = "loop_device_screenshot";
        req_dto->msgid = 0;
        req_dto->data->deviceid = this->device_id_;
        const auto res = this->node_client->api_client->stop_streaming(req_dto);
        const auto res_dto = res->readBodyToDto<oatpp::Object<dto::device_id_response_dto> >(
            this->node_client->obj_mapper);
        this->node_client->assert_res_dto_status(res_dto);
    }

    auto imouse_device_client::set_frame_callback(std::function<void(std::vector<std::byte>)> cb) -> void {
        this->frame_callback = std::move(cb);
    }
}
