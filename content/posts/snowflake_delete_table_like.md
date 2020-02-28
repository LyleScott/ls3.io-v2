---
title: "Snowflake DELETE TABLE LIKE workaround"
date: 2020-02-22T10:12:34-05:00
tags:
  - "data engineering"
  - "snowflake"
  - "sql"
meta_kw:
  - "snowflake"
  - "delete"
  - "table"
  - "delete table"
  - "like"
---

Since `drop tables like '%_staging'` isn't implemented it seems like, I always open up the Snowflake web console and fall back to good 'ol:

{{< gist LyleScott e9ef47ecea98c7b8918d1ed74bf0abd4 >}}

