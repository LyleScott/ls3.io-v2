---
title: "Get Size and Row Count of MySQL Database and Tables"
description: ""
date: 2013-12-30T00:42:06-04:00
tags:
  - "programming"
meta_kw:
  - "mysql"
  - "rowcount"
  - "database size"
  - "table size"
---

## List all databases with row count and size

```sql
SELECT
    table_schema `DB`,
    SUM(table_rows) `Row Count`,
    SUM(data_length + index_length)/1024/1024 `Size (MB)`
FROM
    information_schema.tables
GROUP BY
    table_schema
ORDER BY
    table_schema;
```

```shell
+--------------------+-----------+--------------+
| DB                 | Row Count | Size (MB)    |
+--------------------+-----------+--------------+
| information_schema |      NULL |   0.00878906 |
| intellitype        |         2 |   0.01562500 |
| intellitypesite    |        35 |   0.37500000 |
| jobfoo             |      8048 |   3.34375000 |
| mysql              |      2059 |   0.64330387 |
| performance_schema |     23014 |   0.00000000 |
| myapp              |   2940747 | 263.10937500 |
+--------------------+-----------+--------------+
```

You can add a `WHERE table_schema='YOUR_DATABASE_NAME'` to filter on a database name...

## List all tables in database with row count and size

```sql
SELECT
    table_name,
    table_rows,
    (data_length + index_length)/1024/1024 `Size (MB)`
FROM
    information_schema.tables
WHERE
    table_schema='myapp'
ORDER BY
    table_name;
```

```shell
+----------------------------+------------+--------------+
| table_name                 | table_rows | Size (MB)    |
+----------------------------+------------+--------------+
| auth_group                 |          0 |   0.03125000 |
| auth_group_permissions     |          0 |   0.06250000 |
| auth_permission            |         27 |   0.04687500 |
| auth_user                  |          0 |   0.03125000 |
| auth_user_groups           |          0 |   0.06250000 |
| auth_user_user_permissions |          0 |   0.06250000 |
| django_admin_log           |          0 |   0.04687500 |
| django_content_type        |          9 |   0.03125000 |
| django_session             |          0 |   0.03125000 |
| restapi_place              |       1881 |   0.35937500 |
| restapi_prediction         |    2938332 | 262.28125000 |
| restapi_region             |        279 |   0.06250000 |
+----------------------------+------------+--------------+
```
