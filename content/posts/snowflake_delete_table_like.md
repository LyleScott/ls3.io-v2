---
title: "Snowflake DELETE TABLE LIKE workaround"
date: 2020-02-22T10:12:34-05:00
---

Since `drop tables like '%_staging'` isn't implemented it seems like, I always open up the Snowflake web console and fall back to good 'ol 

```
SELECT CONCAT('DROP TABLE ', table_name, ' ;')
FROM mytable.information_schema.tables 
WHERE table_name like '%_STAGING';
```

... and then click "COPY" to get the TSV results... which is a direct copy/paste into the terminal of the `DROP TABLE` statements I need.
