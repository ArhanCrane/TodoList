#pragma once

#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/utils/daemon_run.hpp>
#include <string_view>
 
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include <userver/utest/using_namespace_userver.hpp>
#include <fmt/format.h>

namespace samples_postgres_service::pg {
    class CrudToDoList final : public server::handlers::HttpHandlerBase {
    public:
        //The name in the static_config.yaml
        static constexpr std::string_view kName = "to-do-list-handler";

        
            //Constuctor, which initialize the HttpHandlerBase and the postgres database
        
        CrudToDoList(const components::ComponentConfig& config, const components::ComponentContext& context);

        using HttpHandlerBase::HttpHandlerBase;
 
        //std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

        std::string HandleRequest(server::http::HttpRequest&, server::request::RequestContext&) const override;
    private:
        std::string GetToDoList(std::string_view, std::string_view, const server::http::HttpRequest&) const;
        std::string PostToDoList(std::string_view, std::string_view, const server::http::HttpRequest&) const;
        std::string DeleteToDoList(std::string_view, std::string_view) const;
        std::string PutToDoList(std::string_view, std::string_view, const server::http::HttpRequest&) const;

        storages::postgres::ClusterPtr pg_cluster_;
    };
}