---
title: "Get Latest Values for an entity/timestamp/value Table"
date: 2020-01-18T22:14:36-05:00
---

In a table of timestamped metric values, it's often useful to ask _what are the latest values_.

This would get the "latest" values across the whole table:

```sql
SELECT device_id, timestamp, reading
FROM sensor_readings t1
WHERE timestamp = (
    SELECT MAX(timestamp)
    FROM sensor_readings t2
    WHERE t1.device_id = t2.device_id
)
ORDER BY device_id, timestamp;
```

