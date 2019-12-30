---
title: "Load & stress testing tools"
description: ""
date: "2017-01-26"
tags:
  - "devops"
meta_kw:
  - "load"
  - "stress"
  - "qps"
  - "request per second"
  - "metrics"
---

I've been doing more and more "load" testing to go along with my architecture and dev work. To me,
it's another way to assert to myself that I've either accomplished a nice solution or I can be
aware of new [tech debt](https://en.wikipedia.org/wiki/Technical_debt) I've just created.

I'm also very interested in plugging up a load testing pipeline to incorporate into my various
[continuous deployment](https://en.wikipedia.org/wiki/Continuous_delivery) and
[continuous integeration](https://en.wikipedia.org/wiki/Continuous_integration) pipelines.
What I'd like to get out of it is a way to tell that I've introduced a performance regression or
I can look at the performance trend over time to draw various conclusions. You could fail the
build on a perf regression, but b warned perf tests can be fickle beasts rife with anomolies...
especially on _cloud_ hardware which is oversubscribed. The long term trend is very benificial for
capacity planning and to just have a finger on the pulse of your app (so you can sleep at night
before the next software demo).

In any case, load/stress testing has many benifits in many scenarios. There are also endless tools,
both free and paid, that help you scale, report on, and repeat large or complex workflows to
exercise your code and server demons. 

Some tools that I turn to:

* [wrk](https://github.com/wg/wrk) - a HTTP benchmarking tool
* [Gatling](http://gatling.io) - load testing and reporting tool
* [Vegeta](https://github.com/tsenart/vegeta) - HTTP load testing tool and library
* [MZBench](https://github.com/machinezone/mzbench) - Multi protocol / plugin load testing tool
* [JMeter](https://jmeter.apache.org) - GUI/cli load testing, functional behaviors, and measuring performance

I won't go into the details, but there is a lot of overlap in these tools. Once you use them
enough, you'll get a feel for their strengths and weaknesses.

There are also services to help if paying rather than building is your thing.

* https://www.blitz.io

