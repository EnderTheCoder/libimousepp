//
// Created by ender on 25-4-10.
//

#ifndef DEVICE_DTO_HPP
#define DEVICE_DTO_HPP
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include OATPP_CODEGEN_BEGIN(DTO)
namespace imouse::dto{
    class device_dto final : public oatpp::DTO {
        DTO_INIT(device_dto, oatpp::DTO)

        DTO_FIELD(String, ip);               // 设备ip地址
        DTO_FIELD(Int32, airhandle);         // 当前连接投屏服务句柄(内部使用)
        DTO_FIELD(String, mac);              // 设备MAC地址
        DTO_FIELD(String, username);         // 设备上关于本机定义的名称
        DTO_FIELD(String, version);          // 手机系统版本
        DTO_FIELD(String, model);            // 设备内部型号
        DTO_FIELD(String, deviceid);         // 设备id
        DTO_FIELD(String, srvname);          // 连接的投屏服务名称
        DTO_FIELD(String, width);            // 手机屏幕真实宽
        DTO_FIELD(String, height);           // 手机屏幕真实高
        DTO_FIELD(String, device_name);      // 手机型号
        DTO_FIELD(String, location);         // usb设备鼠标参数
        DTO_FIELD(String, location_crc);     // usb鼠标参数crc
        DTO_FIELD(Int32, state);             // 0不在线，非0在线
        DTO_FIELD(Int32, rotate);            // 屏幕方向，0竖屏1横屏
        DTO_FIELD(String, vid);              // usb设备vid
        DTO_FIELD(String, pid);              // usb设备pid
        DTO_FIELD(String, gid);              // 分组id
        DTO_FIELD(String, name);             // 设备名(用户自定义名称)
    };
}

#include OATPP_CODEGEN_END(DTO)

#endif //DEVICE_DTO_HPP
