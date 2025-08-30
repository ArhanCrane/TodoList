#include "user_sign_in_out.hpp"

//  /register?name=Andy&password=xxXX1234, /login?name=Andy&password=xxXX1234
samples_postgres_service::pg::RegisterHandler::RegisterHandler(const components::ComponentConfig& config, const components::ComponentContext& context)
: server::handlers::HttpHandlerBase(config, context), pg_cluster_(context.FindComponent<components::Postgres>("postgres-db-1").GetCluster()) {}

std::string samples_postgres_service::pg::RegisterHandler::HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const {
    const auto& name = request.GetArg("name");
    const auto& password = request.GetArg("password");
 
    storages::postgres::Transaction transaction =
        pg_cluster_->Begin("sample_transaction_insert_key_value", storages::postgres::ClusterHostType::kMaster, {});
 
    auto res = transaction.Execute(
        "INSERT INTO hello_schema.users (name_, password_)"
        "VALUES($1, $2)"
        "ON CONFLICT DO NOTHING", name, password);

    if (res.RowsAffected()) {
        transaction.Commit();
        request.SetResponseStatus(server::http::HttpStatus::kCreated);
        return "the user is registered\n";
    }
 
    res = transaction.Execute(
        "INSERT INTO hello_schema.users (name_, password_)"
        "VALUES($1, $2)"
        "ON CONFLICT DO NOTHING", name, password);
    transaction.Rollback();
 
    /*auto result = res.AsSingleRow<std::string>();
    if (result != description) {
        request.SetResponseStatus(server::http::HttpStatus::kConflict);
    }*/
 
    return "default text";
    //return res.AsSingleRow<std::string>();
}       

    
    
    

samples_postgres_service::pg::LoginHandler::LoginHandler(const components::ComponentConfig& config, const components::ComponentContext& context)
: server::handlers::HttpHandlerBase(config, context), pg_cluster_(context.FindComponent<components::Postgres>("postgres-db-1").GetCluster()) {}

std::string samples_postgres_service::pg::LoginHandler::HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const {
    const auto& name = request.GetArg("name");
    const auto& password = request.GetArg("password");

    const storages::postgres::ResultSet res =
    pg_cluster_->Execute(storages::postgres::ClusterHostType::kSlave,
    "SELECT password_ FROM hello_schema.users WHERE name_=$1",
    name);

    if (res.IsEmpty()) {
        request.SetResponseStatus(server::http::HttpStatus::kNotFound);
        return {};
    }

    if (res.AsSingleRow<std::string>() == password) {
        return "the entry is successful\n";
    }else {
        return "the password is incorrect\n";
    }

    /*const storages::postgres::ResultSet res = 
    pg_cluster_->Execute(storages::postgres::ClusterHostType::kSlave, 
    "SELECT description_ FROM hello_schema.items WHERE title_=$1",
    title);
    
    if (res.IsEmpty()) {
        request.SetResponseStatus(server::http::HttpStatus::kNotFound);
        return {};
    }
    return fmt::format("{}\n",res.AsSingleRow<std::string>());*/
}
