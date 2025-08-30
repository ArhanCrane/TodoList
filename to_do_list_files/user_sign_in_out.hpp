#pragma once

//  /register?name=Andy&password=xxXX1234, /login?name=Andy&password=xxXX1234

#include <userver/components/component_list.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include <userver/utest/using_namespace_userver.hpp>

#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>

#include <userver/utest/using_namespace_userver.hpp>
#include <fmt/format.h>

namespace samples_postgres_service::pg {
    class RegisterHandler final : public server::handlers::HttpHandlerBase {
    public:
        static constexpr std::string_view kName = "user-registration-handler";

        RegisterHandler(const components::ComponentConfig& config, const components::ComponentContext& context);
        std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;
    private:
        storages::postgres::ClusterPtr pg_cluster_;
    };

    class LoginHandler final : public server::handlers::HttpHandlerBase {
    public:
        static constexpr std::string_view kName = "user-login-handler";

        LoginHandler(const components::ComponentConfig& config, const components::ComponentContext& context);
        std::string HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;
    private:
        storages::postgres::ClusterPtr pg_cluster_;
    };
}