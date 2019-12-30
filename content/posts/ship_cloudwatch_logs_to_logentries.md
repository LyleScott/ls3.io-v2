---
title: "Ship AWS CloudWatch Logs to Logentries using a Go Lambda"
date: 2018-03-23T22:55:28-04:00
tags:
  - "aws"
  - "devops"
  - "golang"
meta_kw:
  - "cloudwatch"
  - "golang"
  - "logentries"
  - "lambda"
  - "log stream"
  - "log shipper"
  - "aws"
  - "lambda"
---

I've used [Logentries](https://logentries.com) at work and liked it so much that I have started to
use it on my personal projects. It comes in handy when you're looking for a hosted solution for
central logging; a place to push all logs and log messages to so that they are aggregated in
a single place. This allows for easy searching, pretty dashboards, and you can set up various
alerts when patterns you define are matched (ie, an Exception condition).

In my case, I'm working on a personal project that involves a ever growing number of
[AWS Lambda](https://aws.amazon.com/lambda/) functions. I often have to dig into the logs and it's
a pain to drill down into each individual [CloudWatch](https://aws.amazon.com/cloudwatch/) log for
each Lambda. On top of that, there are multiple environments that hold their own set of these
functions so the problem quickly multiplies. This means I waste more time digging for stuff instead
of having fun coding.

You _could_ use application logging in your Lambda code and use your favorite language's Logentries
module or POST directly to their TCP endpoints, but then you would miss out on all the Lambda
specific (ie, outside of the code) log messages; start/end times, cost amounts, timeouts, etc.
Luckily, AWS lets you stream CloudWatch logs to
[Lambda](https://docs.aws.amazon.com/AmazonCloudWatch/latest/logs/Subscriptions.html) and
[Elastic Search](https://docs.aws.amazon.com/AmazonCloudWatch/latest/logs/CWL_ES_Stream.html). So,
if I can stream the logs to Lambda, then I can forward them to
[Logentries via TCP](https://docs.logentries.com/docs/input-token/). To do this effeciently
(which means cheaply...), I'll write this
[Lambda in Go](https://aws.amazon.com/blogs/compute/announcing-go-support-for-aws-lambda/). Giving
me a spot in the code to decide how I want that log line to be processed is pretty powerful.

> You'll need to have signed up for Logentries and have the key handy for the Log you created within Logentries.

## Lambda Code

Thanks to [github.com/bsphere/le_go](https://github.com/bsphere/le_go), most of the boilerplate
for talking to Logentries is taken care of. The Lambda code then becomes a way to transform log
messages into your desired format and to do any _business logic_ you want.

> Don't forget to replace your Logentries Log key in the code.

{{< gist LyleScott fc4d5caef39b59b07f0ee519858d74e9 >}}

Build it to create a binary that will be deployed to the Lambda.

> The `GOOS=linux` bit is important here. It allows you to compile your Go program on a non-linux operating system but will still be compatible for running in the context of a Lambda.

```shell
GOOS=linux go build -o main main.go
zip deployment.zip main
```

## CloudForm Lambda

You may set this Lambda up manually or use your own tool for setting up the infrastructure in an
automated fashion. When I am in AWS land, I default to
[Cloud Formation](https://aws.amazon.com/cloudformation/). Combined with
[AWS Serverless Application Model (SAM)](https://docs.aws.amazon.com/lambda/latest/dg/serverless_app.html),
the infrastructure and application code are a tightly coupled unit and can be deployed very easily
together.

For our example, we will put all the Cloud Formation details in `formation.yml`.

> We've set the [subscription filter](https://docs.aws.amazon.com/AmazonCloudWatch/latest/logs/CreateSubscriptionFilter.html) to trigger on / export the entire line. Basically, a wild card. If you have a specific pattern of messages you care about, you can tune this filter rather than write code to do it. The problem is when you want to filter on multiple patterns.

{{< gist LyleScott 537da996cfafcaf886eda9c0be3b3be1 >}}

> Make sure you set your [S3](https://aws.amazon.com/s3/)  bucket name. See `<NAME-OF-S3-BUCKET>`

```shell
# Variable.
bucket=<NAME-OF-S3-BUCKET>

# Create binary that we told Lambda to use as the handler (via Cloud Formation).
GOOS=linux go build -o main main.go
zip deployment.zip main

# Trigger a Cloud Formation update to update the infrastructure and the deployed code.
aws s3 mb s3://$(bucket)
aws cloudformation package \
    --template-file formation.yml \
    --output-template-file formation.compiled.yml \
    --s3-bucket $(bucket)
aws cloudformation deploy \
    --template-file formation.compiled.yml \
    --stack-name $(bucket) --capabilities CAPABILITY_IAM

# Cleanup
rm -f deployment.zip main
```

## Test It Out

Log into the AWS Console and navigate to the Lambda list page. Go to the LogGeneratorLambda and
and you'll see a "Test" button. Click it, name the default payload it recommends you use (since
the payload is irrelevant), save. You should then be able to click "Test" and it should fire
the Lambda; you will know it was successful by going to the Monitoring tab on the Lambda page and
observing Invocation Count.

Since the Log Generator Lambda has been triggered, this would have written some Cloud Watch logs.
Given that we have the Log Shipper Lambda subscribed to this Log Group, it should too have fired
and theoretically Log Entries should have all log messages.

Navigate to the Log Shipper Lambda and observe that it has some Invocation Count hits on the
Monitoring tab. If there was an Invocation Error, check the logs.

Finally, check Logentries. Hopefully you'll see some stuff there for the Log you specified.

So... time to hook it up to something more fun and nail down the structure that you want.
