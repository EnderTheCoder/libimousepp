//
// Created by ender on 25-4-10.
//

#include "client.hpp"
#include "dto/api_request_dto.hpp"
#include "dto/api_response_dto.hpp"
#include <algorithm>

namespace imouse {
    void imouse_node_client::imouse_ws_instance_listener::onAfterCreate(const WebSocket &socket,
                                                                        const std::shared_ptr<const ParameterMap> &
                                                                        params) {
        const auto listener = std::make_shared<imouse_ws_listener>();
        socket.setListener(listener);
    }

    void imouse_node_client::imouse_ws_instance_listener::onBeforeDestroy(const WebSocket &socket) {
    }

    void imouse_node_client::imouse_ws_listener::onPing(const WebSocket &socket, const oatpp::String &message) {
    }

    void imouse_node_client::imouse_ws_listener::onPong(const WebSocket &socket, const oatpp::String &message) {
    }

    void imouse_node_client::imouse_ws_listener::onClose(const WebSocket &socket, v_uint16 code,
                                                         const oatpp::String &message) {
    }

    void imouse_node_client::imouse_ws_listener::readMessage(const WebSocket &socket, const v_uint8 opcode,
                                                             const p_char8 data,
                                                             const oatpp::v_io_size size) {
        if (opcode == oatpp::websocket::Frame::OPCODE_CLOSE) {
            socket.sendClose();
            return;
        }
        try {
            if (size == 0) {
                const auto buffer_size = buffer.getCurrentPosition();
                buffer.setCurrentPosition(0);
                const auto bytes = std::span{buffer.getData(), static_cast<std::size_t>(buffer_size)};
                if (buffer_size < 265) {
                    throw std::invalid_argument(std::format("expecting frame buffer size to be larger than 265, got {}",
                                                            buffer_size));
                }
                const auto deviceid_data = reinterpret_cast<const char *>(bytes.data());
                std::string deviceid(deviceid_data, 261);
                deviceid = deviceid.c_str();

                int jpg = 0;
                std::array<std::byte, 4> jpg_buffer;
                std::reverse_copy(bytes.data() + 261, bytes.data() + 265, jpg_buffer.begin());
                jpg = *reinterpret_cast<const int32_t *>(jpg_buffer.data());

                const auto img_buffer = std::vector<std::byte>(bytes.data() + 265, bytes.data() + buffer_size);
            } else if (size > 0) {
                buffer.writeSimple(data, size);
            }
        } catch (const std::exception &e) {
            OATPP_LOGE("[imouse::imouse_node_client::imouse_ws_listener::readMessage()]",
                       "Error while reading message: %s", e.what())
        }
    }

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

    auto imouse_device_client::start_streaming(const int interval, const bool jpg) const -> void {
        const auto req_dto = dto::start_streaming_request::createShared();
        req_dto->fun = "loop_device_screenshot";
        req_dto->msgid = 0;
        req_dto->data->isjpg = jpg;
        req_dto->data->time = interval;
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
