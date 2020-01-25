//
// Created by kiosk on 2020/1/23.
//
#include "provider.h"


Provider::Provider() {


    auto fp = fopen("/etc/my_web.json", "rb");
    auto Buffer = std::make_unique<char[]>(4096);

    rapidjson::FileReadStream Read(fp, Buffer.get(), 4096);
    fclose(fp);
    rapidjson::Document doc;
    doc.ParseStream(Read);
    m_port = doc["server"]["port"].GetInt();
    m_keep_connection_ms = doc["server"]["keep_connection_ms"].GetInt();
    m_keep_connection_number = doc["server"]["keep_connection_number"].GetInt();
    m_threads = doc["server"]["threads"].GetUint();
    m_threads = m_threads > std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : m_threads;
    m_ip = doc["server"]["ip"].GetString();
    for (auto &&i : doc["sites"].GetArray()) {
        m_wwwroot = i["wwwroot"].GetString();
        m_default_file = i["default_file"].GetString();

        for (const auto &p : i["fastcgi"].GetArray()) {
            m_FastCgi_Port = p["port"].GetUint();
            m_FastCgi_ip = p["ip"].GetString();
        }
    }

}
