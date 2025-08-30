#include "crud_operation.hpp"

#include <string>


samples_postgres_service::pg::CrudToDoList::CrudToDoList(const components::ComponentConfig& config, const components::ComponentContext& context) 
    : server::handlers::HttpHandlerBase(config, context),
    pg_cluster_(context.FindComponent<components::Postgres>("postgres-db-1").GetCluster()) {}

std::string samples_postgres_service::pg::CrudToDoList::HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const {
    const auto& name = request.GetArg("name");
    const std::string_view& title = request.GetArg("title");

    
    //    It throws an exception (similar to 400) if title is empty
    
    if (title.empty()) {
        throw server::handlers::ClientError(server::handlers::ExternalBody{"No 'title' query argument"});
    }
    request.GetHttpResponse().SetContentType(http::content_type::kTextPlain);
    switch (request.GetMethod()) {
        case server::http::HttpMethod::kGet:
            return samples_postgres_service::pg::CrudToDoList::GetToDoList(name, title, request);
        case server::http::HttpMethod::kPost:
            return samples_postgres_service::pg::CrudToDoList::PostToDoList(name, title, request);
        case server::http::HttpMethod::kDelete:
            return samples_postgres_service::pg::CrudToDoList::DeleteToDoList(name, title);
        case server::http::HttpMethod::kPut:
            return samples_postgres_service::pg::CrudToDoList::PutToDoList(name, title, request);
        default:
            throw server::handlers::ClientError(server::handlers::ExternalBody{fmt::format("unsupported method - {}", request.GetMethod())});
    }
}

std::string samples_postgres_service::pg::CrudToDoList::GetToDoList(std::string_view name, std::string_view title, const server::http::HttpRequest& request) const {
    const storages::postgres::ResultSet res = 
    pg_cluster_->Execute(storages::postgres::ClusterHostType::kSlave,     
        "SELECT title_, description_ FROM hello_schema.items WHERE name_=$1",
    name);
    
    if (res.IsEmpty()) {
        request.SetResponseStatus(server::http::HttpStatus::kNotFound);
        return {};
    }
    std::string output{};

    for (auto row : res.AsSetOf<std::tuple<std::string, std::string>>(storages::postgres::kRowTag)) {
        static_assert(std::is_same_v<decltype(row), std::tuple<std::string, std::string>>,
        "Iterate over tuples");
        auto [a, b] = row;
        output += a;
        output += " ";
        output += b;
        output += "\n";
    }
     
    return output;    
}

std::string samples_postgres_service::pg::CrudToDoList::PostToDoList(std::string_view name, std::string_view title, const server::http::HttpRequest& request) const {    
    const auto& description = request.GetArg("value");    
    storages::postgres::Transaction transaction =
        pg_cluster_->Begin("sample_transaction_insert_key_value", storages::postgres::ClusterHostType::kMaster, {});
 
    auto res = transaction.Execute(
        "INSERT INTO hello_schema.items (name_, title_, description_)"
        "VALUES($1, $2, $3)"
        "ON CONFLICT DO NOTHING", name, title, description);
    if (res.RowsAffected()) {
        transaction.Commit();
        request.SetResponseStatus(server::http::HttpStatus::kCreated);
        return "the operation is successful\n";
    }
 
    res = transaction.Execute(
        "INSERT INTO hello_schema.items (name_, title_, description_)"
        "VALUES($1, $2, $3)"
        "ON CONFLICT DO NOTHING", name, title, description);
    transaction.Rollback();
 
    auto result = res.AsSingleRow<std::string>();
    if (result != description) {
        request.SetResponseStatus(server::http::HttpStatus::kConflict);
    }
 
    return res.AsSingleRow<std::string>();
}

std::string samples_postgres_service::pg::CrudToDoList::DeleteToDoList(std::string_view name, std::string_view title) const {
    auto res = pg_cluster_->Execute(storages::postgres::ClusterHostType::kMaster,
    "DELETE FROM hello_schema.items WHERE name_=$1 and title_=$2",
    name, title);
    return std::to_string(res.RowsAffected());
}

std::string samples_postgres_service::pg::CrudToDoList::PutToDoList(std::string_view name, std::string_view title, const server::http::HttpRequest& request) const {
    const auto& description = request.GetArg("value");    
    storages::postgres::Transaction transaction =
        pg_cluster_->Begin("sample_transaction_change_key_value", storages::postgres::ClusterHostType::kMaster, {});
 
    auto res = transaction.Execute(
        "UPDATE hello_schema.items SET description_ = $3 WHERE title_ = $2 and name_ = $1;",
        name, title, description);

    if (res.RowsAffected()) {
        transaction.Commit();
        request.SetResponseStatus(server::http::HttpStatus::kCreated);
        return "the operation is successful\n";
    }
 
    res = transaction.Execute(
        "UPDATE hello_schema.items SET description_ = $3 WHERE title_ = $2 and name_ = $1;",
        name, title, description);
    transaction.Rollback();
 
    //auto result = res.AsSingleRow<std::string>();
    /*if (result != description) {
        request.SetResponseStatus(server::http::HttpStatus::kConflict);
    }*/
 
    //return res.AsSingleRow<std::string>();
    return "Done!";
}