DROP SCHEMA IF EXISTS hello_schema CASCADE;

CREATE SCHEMA IF NOT EXISTS hello_schema;

CREATE TABLE IF NOT EXISTS hello_schema.users (
    name_ VARCHAR(32) PRIMARY KEY,
    password_ VARCHAR(64)
);


CREATE TABLE IF NOT EXISTS hello_schema.items (    
    name_ VARCHAR(32),
    title_ VARCHAR(64) PRIMARY KEY,
    description_ VARCHAR(64),

    FOREIGN KEY (name_) REFERENCES hello_schema.users(name_)
);
