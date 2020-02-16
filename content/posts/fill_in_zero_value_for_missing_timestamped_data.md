---
title: "Fill in Zero Values for Timestamped Readings from Sensors in Snowflake"
description: ""
date: 2020-02-16T13:54:10-05:00
tags:
  - "programming"
  - "snowflake"
meta_kw:
  - "fill in"
  - "snowflake"
  - "zero values"
  - "zeros"
---

I was trying to come up with a [Snowflake](https://www.snowflake.com/) SQL query to give me all readings between `2020-01-01 00:00:00` and `2020-01-01 00:00:03` and report a `0` value for each `sensor_id`/`timestamp` missing from the query.

So, say the database table is populated like so:

| timestamp           | sensor_id | reading |
| ------------------- | --------- | ------- |
| 2020-01-01 00:00:00 | sensor1   | 1.23    |
| 2020-01-01 00:00:01 | sensor2   | 1.23    |

The intended end result will be a `0` value for the `timestamp`/`sensor_id` pair where there is no data.

| timestamp           | sensor_id | reading |
| ------------------- | --------- | ------- |
| 2020-01-01 00:00:00 | sensor1   | 1.23    |
| 2020-01-01 00:00:00 | sensor2   | 0       |
| 2020-01-01 00:00:01 | sensor1   | 0       |
| 2020-01-01 00:00:01 | sensor2   | 1.23    |
| 2020-01-01 00:00:02 | sensor1   | 0       |
| 2020-01-01 00:00:02 | sensor2   | 0       |

If you wanted to set up a Snowflake DB for this example query, do:

```sql
create or replace table foobar (
    timestamp timestamp_tz(0) not null,
    sensor_id varchar(32),
    reading float not null
);

insert into foobar values
    ('2020-02-14T02:00:00+00:00'::timestamp_tz, 'sensor1', 1.2),
    ('2020-02-14T02:00:01+00:00'::timestamp_tz, 'sensor1', 2.3),
    ('2020-02-14T02:00:03+00:00'::timestamp_tz, 'sensor1', 3.4),
    ('2020-02-14T02:00:03+00:00'::timestamp_tz, 'sensor2', 10.2),
    ('2020-02-14T02:00:09+00:00'::timestamp_tz, 'sensor1', 4.5),
    ('2020-02-14T02:00:09+00:00'::timestamp_tz, 'sensor3', 11.4);
```

The actual query uses some variables to DRY things up a bit. Imagine these coming from user input somehow.

```sql
set granularity='second';
set start_date = '2020-02-14T02:00:00+00:00'::timestamp_tz;
set end_date = '2020-02-14T02:00:10+00:00'::timestamp_tz;
set dd=(select datediff($granularity, $start_date, $end_date));

-- make sure there are "0" `reading` values for
-- all missing datapoints for all sensors

select
    gen_dates.d timestamp,
    gen_dates.sensor_id,
    ifnull(fb.reading, 0) as reading
from (
    select distinct sensor_id, d from foobar
    cross join (
        select dateadd(second, '+' || seq4(), $start_date) as d
        from table (generator(rowcount => $dd))
    )
) gen_dates
left join (
    select *
    from foobar
) fb
on
    fb.timestamp::timestamp_tz = gen_dates.d::timestamp_tz
    and fb.sensor_id::string = gen_dates.sensor_id::string
where
    gen_dates.d between $start_date and $end_date
order by
    gen_dates.d asc,
    gen_dates.sensor_id asc
;
```

