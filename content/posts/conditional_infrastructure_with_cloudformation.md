+++
date = "2017-10-25"
title = "Conditional Infrastructure with AWS Cloud Formation"
description = "Conditionalize parameters, resources, and even stacks!"
tags = [
  "aws",
  "devops"
]
meta_kw = [
  "cloud formation",
  "conditionals",
  "aws",
  "yaml",
  "json",
  "resource",
  "stack"
]
+++

I'd recommend atleast peeking at [the docs](http://docs.aws.amazon.com/AWSCloudFormation/latest/UserGuide/conditions-section-structure.html) if you have not already. There will probably be some stuff in there that is out of the scope of this post.

The following are some usage examples I think [Cloud Formation](https://aws.amazon.com/cloudformation/)
conditionals can really help with.

I'm going to be using YAML here instead of JSON. YAML easily converts into JSON so if you are stuck
in the painful world of JSON, anything discussed here should carry right over. (there are converters).

### A few words on Conditionals

Don't think you can start throwing down programming logic in your Cloud Formation templates as if
you were in your favorite scripting language. As with most things AWS, you must play by their rules
with their tools.

The premise of Conditionals are you declare them up front in a `Conditionals` block and you use
them in the `Resources` or `Outputs` blocks to create bits of logic using `If`/`Then` type logic.
It can get messy if your logic is nested, but sometimes you can use (abuse?) conditionals a bit
so your logic with nested `If`/`Then` constructs are atleast more readable. For example, rolling
up `Or` logic in the conditional rather than where you are evaluating it.

### Conditional Variables

This concept seems to be the common case in the AWS docs. Nothing really exciting here.

This example illustrates a Conditional that is truthy when a ParameterKey=Environment and
ParameterValue is either prod or demo. Otherwise, it's always falsy.

Cloud Formation template: `formation.yml`

```yaml
AWSTemplateFormatVersion: 2010-09-09
Parameters:

  Environment:
    Type: String

Conditions:

  # Look at that super fancy Conditional!
  IsProdLike: !Or
    - !Equals [ !Ref Environment, "prod" ]
    - !Equals [ !Ref Environment, "demo" ]

Resources:

  MyBucket:
    Type: "AWS::S3::Bucket"
    Properties:
      BucketName: !Sub "${AWS::AccountId}-cf-params"
      VersioningConfiguration:
        # Using the Conditional to set a variable in a specific way.
        Status: !If [ IsProdLike, "Enabled", "Suspended" ]

```

Parameters file: `parameters.json`

```json
[
    {
        "ParameterKey": "Environment",
        "ParameterValue": "foobar"
    }
]
```

Create the stack:

```bash
aws cloudformation create-stack \
    --capabilities CAPABILITY_NAMED_IAM \
    --stack-name cf-param-conditional \
    --template-body file://formation.yml \
    --parameters file://parameters.json
```

Check the file versioning:

```bash
# Stash the AWS account ID in a variable to build the S3 bucket name.
account_id=$(aws sts get-caller-identity --query 'Account' --output text)

# Query the S3 API to get the versioning status. (should be Suspended)
aws s3api get-bucket-versioning --bucket ${account_id}-cf-params
```

To verify I'm not full of it, change the `Environment` Parameter to be `prod` or `demo`. This
should enable S3 versioning for that bucket.

```bash
# Set the environment to "prod"
sed -i '' s/foobar/prod/ parameters.json   # OS X, BSD
sed s/foobar/prod/ parameters.json         # Linux

# Update the Cloud Formation stack.
aws cloudformation update-stack \
    --stack-name cf-param-conditional \
    --template-body file://formation.yml \
    --parameters file://parameters.json

# Query the S3 API to get the versioning status. (should be Enabled)
aws s3api get-bucket-versioning --bucket ${account_id}-cf-params
```

Clean up:

```bash
aws cloudformation delete-stack --stack-name cf-param-conditional
```

### Conditional Resources

So, this is where it starts to get interesting. It's cool to do a bit of logic within a template so
you don't have to hardcode so many things. It also allows you to have a monolithic template for a
service rather than force you into evil non-DRY ways like having a template for each specific
use-case.

Taking this one step further, we can say "we want this resource to be created under a specific
circumstance". This is very handy for developer vs prod like environments where you want to
optimize for cost and/or reduce the time it takes to stand up a new stack.

For example, suppose you have a Aurora RDS database in a Master/Slave setup for a prod instance. A
developer wants to recreate some infrasturcture to start repeating a problem out of band from
production. What this will lead to is a developer spinning up their own stack to mimick prod
components. Really, does the dev _really_ need a slave DB? It costs a bit more money and it adds
quite a bit of time to set up and tear down (RDS takes forrreeevvverr).

I like to use a `${Service}Enabled` pattern where I have a Parameter dictate what Conditional
resources get stood up.

The `formation.yml` could use the Parameters across one or more Resources.

```yml
AWSTemplateFormatVersion: 2010-09-09
Parameters:

  SlaveEnabled:
    Type: String
    Default: "true"

Conditions:

  IsSlaveEnabled: !Equals [ !Ref SlaveEnabled, "true" ]

Resources:

  DbBackupBucket:
    Type: "AWS::S3::Bucket"
    Condition: IsSlaveEnabled
    Properties:
      BucketName: !Sub "${AWS::AccountId}-${AWS::Region}-database-backups"
```

Create the `parameters.json` file with:

```json
[
    {
        "ParameterKey": "SlaveEnabled",
        "ParameterValue": "false"
    }
]
```

Create the stack:

```bash
aws cloudformation create-stack \
    --capabilities CAPABILITY_NAMED_IAM \
    --stack-name cf-resource-conditional \
    --template-body file://formation.yml \
    --parameters file://parameters.json
```

Notice you should only have 1 DB instance:

```bash
aws rds describe-db-instances \
    --query 'DBInstances[?MasterUsername==`resource_conditionals`]'
```

Change the `ParameterValue` for `SlaveEnabled` to be `true` and update the stack.

```bash
# Set SlaveEnabled to "true"
sed -i '' s/false/true/ parameters.json   # OS X, BSD
sed s/false/true/ parameters.json         # Linux

aws cloudformation update-stack \
    --stack-name cf-resource-conditional \
    --template-body file://formation.yml \
    --parameters file://parameters.json
```

After waiting a bit (hit the Cloud Formation page in the Console to check status), you'll notice
there's a replica/slave stood up!

```bash
aws rds describe-db-instances \
    --query 'DBInstances[?MasterUsername==`resource_conditionals`]'
```

Clean up:

```bash
aws cloudformation delete-stack --stack-name cf-resource-conditional
```


### Conditional Stacks

Thus far we've implemented Parameter value logic and Resource creation logic. Let's unpeel the
onion one more layer!

This is sort of a combo feature since it requires you to also be using the
[AWS::CloudFormation::Stack](http://docs.aws.amazon.com/AWSCloudFormation/latest/UserGuide/aws-properties-stack.html) 
primitive. I've [written about it here](/post/conditional_infrastructure_with_cloudformation/) for
more info on it.

A `Condition` on a Stack is a great building block for à la carte infrastructure; meaning you can
easily flag out stacks so you can isolate or easily choose which parts of a stack you want to
stand up.

For example, suppose you have a few Stack definitions already:

- API (RDS, EC2, S3 bucket for uploads) 
- Analytics Pipeline (Kinesis Input Stream, Kinesis Analytics App, Kinesis Firehose --> S3)
- Frontend (S3 bucket for static assets, Cloud Front distribution for CDN, ACM cert for SSL)

Say you're a developer. You want to do some analytics work but don't care about spinning up an API
stack (time, money, irrelevant) or a Frontend. You just want to get at the Analytics pieces.

Using a `Condition` on a `AWS::CloudFormation::Stack` Resource, we can accomplish this very easily
with the same pattern that was used for a regular 'ol Resource.

For example, take this `formation.yml`:

```yaml
AWSTemplateFormatVersion: 2010-09-09

Parameters:

  ApiEnabled:
    Type: String
    Default: "true"

  AnalyticsEnabled:
    Type: String
    Default: "true"

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
        SomeParameter: "abc123"
      TemplateURL: 'https://s3.amazonaws.com/your-stack-bucket/api.yml'

  Analytics:
    Type: 'AWS::CloudFormation::Stack'
    Condition: IsAnalyticsEnabled
    Properties:
      Parameters:
        SomeParameter: "abc123"
      TemplateURL: 'https://s3.amazonaws.com/your-stack-bucket/analytics.yml'

  Frontend:
    Type: 'AWS::CloudFormation::Stack'
    Condition: IsFrontendEnabled
    Properties:
      Parameters:
        SomeParameter: "abc123"
      TemplateURL: 'https://s3.amazonaws.com/your-stack-bucket/frontend.yml'
```

Then, with a `parameters.json` file like:

```json
[
    {
        "ParameterKey": "ApiEnabled",
        "ParameterValue": "false"
    },
    {
        "ParameterKey": "AnalyticsEnabled",
        "ParameterValue": "true"
    },
    {
        "ParameterKey": "FrontendEnabled",
        "ParameterValue": "false"
    }
]
```

You can begin to see what sort of toggles we can build. Pretty powerful stuff for just a little bit
of YAML config!

**Note:** You _can't_ use multiple conditions for a Resource's `Condition` value. This is lame, I 
know. You need to just get creative with your Condition blocks and use `!And` and `!Or` to create
a single Condition that suits your needs.
