---
title: "Snowflake DELETE TABLE LIKE workaround"
date: 2020-02-22T10:12:34-05:00
---

Since `drop tables like '%_staging'` isn't implemented it seems like, I always open up the Snowflake web console and fall back to good 'ol 

```sql
SELECT CONCAT('DROP TABLE ', table_name, ' ;')
FROM your_database_name.information_schema.tables 
WHERE table_name like '%_STAGING';
```

Then, click "COPY" to get the TSV results. You may then paste the commands into the terminal, select all of them, and execute the queries.
