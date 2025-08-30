#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/clients/dns/component.hpp>

//#include "hello.hpp"
#include "crud_operation.hpp"
#include "user_sign_in_out.hpp"

int main(int argc, char* argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()                            
                            .Append<userver::server::handlers::TestsControl>()
                            .Append<userver::clients::dns::Component>()
                            .Append<samples_postgres_service::pg::CrudToDoList>()
                            .Append<samples_postgres_service::pg::LoginHandler>()
                            .Append<samples_postgres_service::pg::RegisterHandler>()
                            .Append<userver::components::Postgres>("postgres-db-1");                   

  return userver::utils::DaemonMain(argc, argv, component_list);
}