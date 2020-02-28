---
title: "Create a read-only role in Snowflake"
date: 2020-02-28T10:00:00-05:00
tags:
  - "data engineering"
  - "snowflake"
  - "sql"
meta_kw:
  - "snowflake"
  - "readonly"
  - "read only"
  - "role"
  - "user"
  - "access"
  - "permission"
  - "grant"
---

```sql
-- create the read-only role.
create role if not exists YOUR_ROLE_NAME;

-- allow the role to access a specific warehouse
grant usage on warehouse YOUR_WAREHOUSE_NAME to role YOUR_ROLE_NAME;

-- allow the role to access a specific schema (most likely public?)
//grant usage on all schemas in database YOUR_DATABASE_NAME to role YOUR_ROLE_NAME;
grant usage on schema public to role YOUR_ROLE_NAME;

-- allow the role to access a specific database
grant usage on database YOUR_DATABASE_NAME to role YOUR_ROLE_NAME;

-- allow the role access to all tables in a database (and that will be created in future...)
grant select on future tables in database YOUR_DATABASE_NAME to role YOUR_ROLE_NAME;
grant select on all tables in database YOUR_DATABASE_NAME;

-- allow the role access to all views in a database (and that will be created in future...)
grant select on future views in database YOUR_DATABASE_NAME to role YOUR_ROLE_NAME;
grant select on all views in database YOUR_DATABASE_NAME;
```

