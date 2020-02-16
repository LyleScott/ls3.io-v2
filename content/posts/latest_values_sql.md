---
title: "SQL for Getting the Most Recent Values for Time Series"
date: 2020-01-18T22:14:36-05:00
---

Given a table for time-series data, it is often useful to ask _what are the most recent values for each entity_ to get a picture of the state up to a specific time.

For example, take sensor data; tracking attributes of a sensor over time. You might want to query the current state of some sensors.

I work with a lot of IoT data, so this is pretty handy to have at my disposal. Trying to answer the question let me down the following path(s).

## Real World Example

Let's do a little real-world example for something like IoT sensor readings.

> Examples are using Postgres 10.11

Create a small schema to support our queries:

```sql
CREATE TABLE sensor_readings (
    ts          timestamp,
    sensor_id   uuid,
    reading     float
)
```

Insert some initial data to the table so we can see what's going on more visually:

```sql
INSERT INTO sensor_readings VALUES
    ('2020-01-20T13:00:00 +0000', 1, 1),
    ('2020-01-20T14:00:00 +0000', 2, 3),
    ('2020-01-20T14:00:00 +0000', 3, 7),
    ('2020-01-20T15:00:00 +0000', 1, 11),
    ('2020-01-20T16:00:00 +0000', 2, 13),
    ('2020-01-20T17:00:00 +0000', 2, 17);
```

My initial solution for me was a basic correlated subquery.

> Also works in Snowflake.

```sql
SELECT *
FROM sensor_readings t1
where ts = (
    SELECT MAX(ts)
    FROM sensor_readings t2
    WHERE t1.sensor_id = t2.sensor_id
);
```

A DBA friend of mine used `PARTITION BY` which I'm not so familiar with.

```sql
SELECT *
FROM (
    SELECT *, ROW_NUMBER() OVER(PARTITION BY sensor_id ORDER BY ts DESC) AS rn
    FROM sensor_readings
) t2
 WHERE t2.rn = 1;
```

... more clearly written as:

```sql
WITH T AS (
    SELECT *, ROW_NUMBER() OVER(PARTITION BY sensor_id ORDER BY ts DESC) AS rn
    FROM sensor_readings
)
SELECT * FROM T WHERE rn = 1;
```

