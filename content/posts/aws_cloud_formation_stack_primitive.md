+++
date = "2017-10-05"
title = "AWS Cloud Formation AWS::Stack primitive"
description = "Stacks on Stacks on Stacks!"
tags = [
  "aws",
  "devops"
]
meta_kw = [
  "cloud formation",
  "stack",
  "aws::stack",
  "yaml",
  "json",
  "resource",
  "stack"
]
+++

I've finally been able to dig into the `AWS::Stack` primitive! Since discovering it, I've been
refactoring both multi-formation setups for both work and my personal projects. It really was a
crucial missing piece.

### What it gives you

Orchestrating many small pieces of infrastructure can be a huge pain in the ass. Similarly, 
managing a bunch of small AWS Cloud Formation stacks can be painful. Deployments take thought
(gross) and mistakes are more likely to occur rolling out updates. With so many small pieces of
Cloud Formation stuff going on, you'll find yourself in one or more situations:

* Many small stacks are used to build a larger piece of infrastructure
* Building an overly large and complex single formation file that you don't want to split up in
  fear of pieces not getting deployed together
* A deploy process that requires that you deploy services in a specific order
* A loss of transactionality when deploying services; deploy stack a, b, c, problem on d. The only
  way to roll back a, b, c is to undo the commit and redeploy the formation yourself. In reverse
  order!

Utilizing an abstraction layer higher than an individual stack addresses these concerns head-on:

* The deployment story becomes deploying a single stack. This will deploy all nested stacks _and_
  handle the dependency graph much more gracefully.
* Transactional rollouts over multiple services; all work that was done can be rolled back
  if something fails later in the stack update. The power to turn this off is now in your hands as
  well.
* À la carte infrastructure becomes easier;
  see [this post for more info](/post/conditional_infrastructure_with_cloudformation/).

### What it looks like

```yaml
AWSTemplateFormatVersion: 2010-09-09

Parameters:

  ApiEnabled:
    Type: String
    Default: "true"

  ApiDatabaseName:
    Type: String
    Default: api

  AnalyticsEnabled:
    Type: String
    Default: "true"

  AnalyticsDatabaseName:
    Type: String
    Default: analytics 

  FrontendEnabled:
    Type: String
    Default: "true"

Conditions:

  IsApiEnabled: !Equals [ !Ref ApiEnabled, "true" ]
  IsAnalyticsEnabled: !Equals [ !Ref AnalyticsEnabled, "true" ]
  IsFrontendEnabled: !Equals [ !Ref FrontendEnabled, "true" ]

Resources:

  Api:
    Type: 'AWS::CloudFormation::Stack'
    Condition: IsApiEnabled
    Properties:
      Parameters:
        DatabaseName: !Ref ApiDatabaseName
      TemplateURL: 'https://s3.amazonaws.com/your-stack-bucket/api.yml'

  Analytics:
    Type: 'AWS::CloudFormation::Stack'
    Condition: IsAnalyticsEnabled
    Properties:
      Parameters:
        DatabaseName: !Ref AnalyticsDatabaseName
      TemplateURL: 'https://s3.amazonaws.com/your-stack-bucket/analytics.yml'

  Frontend:
    Type: 'AWS::CloudFormation::Stack'
    Condition: IsFrontendEnabled
    Properties:
      TemplateURL: 'https://s3.amazonaws.com/your-stack-bucket/frontend.yml'
```

The `parameters.json` shouldn't really look any different:

```json
[
    {
        "ParameterKey": "ApiDatabaseName",
        "ParameterValue": "foobarbaz"
    },
    {
        "ParameterKey": "AnalyticsDatabaseName",
        "ParameterValue": "bingbangboom"
    }
]
```

### Parameters

Parameters bubble down from the outer formation file down to the child formations. To keep things
clean (and readable!), I tend to namespace my Parameter
names in the outer formation file by prefixing them with the Stack's Resource name. This is needed
in the following example:

Say you have a database1 stack and a database2 stack defined in your formation template. Each
individual stack takes `DatabaseName` as a Parameter (and lets assume they might be different
values). You need to differentiate them in the Parameters supplied to the outer template and I've
found it very clean, albiet verbose, to use the prefix of the Resource of that stack. For example,
the above formation has the following Resources: Api, Analytics, Frontend. So, any Parameters to
the API stack would have the "API" prefix; ie, `ApiDatabaseName`.

### A helpful Makefile

Annoyingly, the child stacks must have their formation file(s) in S3. I tend to create a `Makefile`
(for better or for worse sometimes...) so I have an error-free and limited typing dev cycle.

```bash
profile ?= default
region ?= us-east-1

default: update

define sync_bucket
    aws s3 sync \
        --profile $(profile) \
        --region $(region) \
        --delete \
        formations/ s3://gc-stack-files
    aws cloudformation $(1)-stack \
        --profile $(profile) \
        --region $(region) \
        --stack-name stack \
        --template-body file://formation.yml \
        --parameters file://parameters.json
endef

create:
    aws s3 mb \
        --profile $(profile) \
        --region $(region) \
        s3://gc-stack-files || true
    aws s3api put-bucket-versioning \
        --profile $(profile) \
        --bucket gc-stack-files \
        --versioning-configuration Status=Enabled
    $(call sync_bucket,create)

update:
    $(call sync_bucket,update)

delete:
    aws s3 rb \
        --profile $(profile) \
        --region $(region) \
        s3://gc-stack-files \
        --force
    aws cloudformation delete-stack \
        --profile $(profile) \
        --region $(region) \
        --stack-name stack
```

This allows make to just type `make create` or `make update`.

I also always need support for using other AWS cli profiles and regions. These are supported, too,
by doing something like `make profile=otherprofile region=eu-west-2` (update is implicit default)
or just `make region=eu-west-2 create`.
