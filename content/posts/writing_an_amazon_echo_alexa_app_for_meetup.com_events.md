---
title: "Writing an Amazon Echo/Alexa app for Meetup.com Events"
description: "A small intro to Amazon Echo, AWS Lambda (Python), and the Alexa Toolkit."
date: "2016-10-29"
tags:
  - "aws"
  - "iot"
  - "programming"
  - "python"
meta_kw:
  - "tutorial"
  - "programming"
  - "alexa"
  - "echo"
  - "amazon"
  - "lambda"
  - "AWS"
  - "internet of things"
  - "skill toolkit"
  - "sdk"
  - "meetup.com"
  - "meetup"
  - "API"
  - "hack day"
---

Recently, I had to give an impromptu [Python](https://www.python.org) talk for
[PyStPete](https://www.meetup.com/Saint-Petersburg-Python-Meetup/) since it can be... difficult ...
to get people to volunteer talks. I had just hacked on a pair of
[Amazon Echos to tell each other Yo Momma Jokes](https://twitter.com/digitalfoo/status/789251171345174533)
at a hackday we had [at work](https://www.gooee.com), so I had Amazon Echo hacks on the brain. I
figured, why not do a quick talk on getting started with Amazon Echo in the realm of Python and
throw in a little [AWS Lambda](https://aws.amazon.com/lambda/) foo while I was at it.

## A quick overview of terms

* [Amazon Web Services (AWS)](https://aws.amazon.com/) - Amazon's vast offering of "things" as a service.
* [AWS Lambda](https://aws.amazon.com/lambda/) - Serverless architecture for serving code that interacts with Amazon Echo.
* [AWS Cloud Watch](https://aws.amazon.com/cloudwatch/) - Interface for viewing logs for Lambda code being executed.
* [Alexa Skills Toolkit](https://developer.amazon.com/edw/home.html) - The SDK development portal for configuring Echo apps.
* [GitHub Repo for Example Code](https://github.com/LyleScott/Amazon-Echo-Next-Meetup-App) - An example app you can configure for any meetup.

## Setting up AWS Lambda

You'll actually need an [AWS account](https://aws.amazon.com). This will involve putting in your
billing info (incase you get crazy). This app alone will not cost you anything (most likely).

Create an [IAM](https://console.aws.amazon.com/iam/) Role to access AWS Lambda with.

1. Go to the [IAM page](https://console.aws.amazon.com/iam/).
2. Click *Roles*.
3. Click *Create New Role*
4. Role Name = lambda_basic_execution
5. Click *Next Step*
6. Select *AWS Lambda* as the Role type.
7. Search for and select *AWSLambdaBasicExecutionRole*.
8. Click *Next Step* and *Create Role*.

Create the Lambda function that will execute the code we'll write to interact with Amazon Echo.

1. Go to the [Lambda page](https://console.aws.amazon.com/lambda/).
2. Create a Lambda Function
3. Filter blueprints by "Python"
4. Select *hello-world-python*.
5. To the left of the "Lambda" icon, you'll seee a gray outlined box. Select it and click *Alexa Skills Kit*.
6. Name = SomeFunctionName
7. Description = Some description that makes sense.
8. Runtime = Python 2.7 (I know, why no Python 3?!)
9. Go with Code entry type = *Edit Code Inline* and leave the default for now.
10. Handler = lambda_function.lambda_handler
11. Existing role = lambda_basic_execution
12. Accept any further defaults and click *Next* and *Create Function*.

We now have a Lambda function for use with our Alexa app. 

1. Go to the [Lambda page](https://console.aws.amazon.com/lambda) and you should see your new function.
2. Click it.
3. Look in the top right and you'll see a *ARN* field with a long string of text to the right of it. Something like "arn:aws:lambda:us-east-1:975070732451:function:MyFunction". Save this. You'll need to add this to your Alexa Skills Kit setup.

## Configuring Alexa Skills Kit

Now that we have a Lambda function setup, we can create an Alexa skill that will use it.

1. Go to the [Alexa Skills Kit](https://developer.amazon.com/edw/home.html#/skills/list) page.
2. Click *Add New Skill*.
3. Skill type = Custom Interaction Model
4. Name = Name of the skill that is displayed to customers in the Alexa app.
5. Invocation Name = The command to ask Alexa to do.
6. Click *Next*.
7. Intent Schema = contents of [the example intent schema](https://github.com/LyleScott/Amazon-Echo-Next-Meetup-App/blob/master/misc/intent_schema.txt)
8. Sample Utterances = contents of [the example utterances file](https://github.com/LyleScott/Amazon-Echo-Next-Meetup-App/blob/master/misc/utterances.txt)
9. Click *Next*.
10. Select *AWS Lambda ARN (Amazon Resource Name)*
11. Copy in the ARN of the AWS Lambda you created earlier.
12. Click *Next*.
13. Notice the handy dandy test screen. This is used to simulate talking to your device to facilitate easier testing.
12. Click *Next*.
13. STOP! No need to continue further. Everything will work with your AWS account / Echo. The information left to fill out is only for when you want to publish your app to the Echo app store.

## Push application code to AWS Lambda

Check out and configure the code that handles interactions with AWS Lambda and the Meetup API.

You'll need an API token for the Meetup API. This will be set in the `settings.py` file below.
Assuming you are logged in, you can find your Meetup API token
[here](https://secure.meetup.com/meetup_api/key/).

You'll also need to know the URL of your meetup. The bold bit:
www.meetup.com/**Saint-Petersburg-Python-Meetup**/. This will also be set in `settings.py`.

```bash
# Check out the GitHub repo.
git clone git@github.com:LyleScott/Amazon-Echo-Next-Meetup-App.git

cd Amazon-Echo-Next-Meetup-App

# Copy the template.
cp settings.dist.py settings.py
# ... edit settings.py ...

# You can use a graphical program to create a .zip file.
# Just make sure all files are in the *root* of the zip file.
zip -r app.zip *
```

1. Use the example above to configure/zip files that will be uploaded to AWS Lambda.
2. Go to the [AWS Lambda page](https://console.aws.amazon.com/lambda/).
3. Click on the Lambda function you created earlier.
4. Code entry type = Upload a .ZIP file.
5. Click *Upload* and browse and find app.zip and select it.
6. Click *Save*.

## Test It Out

> Alexa, ask (invocation name) (phrase).

> Alexa, ask Py Saint Pete when the next meetup is.

Hopefully you'll get a valid response. If you get an error, see the
[AWS Cloud Watch](https://aws.amazon.com/cloudwatch/) logs that correspond to your Lambda function.
