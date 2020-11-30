+++
date = "2020-10-05"
title = "My Docker Patterns"
description = "Docker patterns that I find useful"
tags = [
  "docker",
  "devops"
]
meta_kw = [
  "docker",
  "docker tips",
  "docker tutorial",
  "docker help",
  "docker patterns",
  "cleanup",
  "maintenance",
  "local development"
]
+++

These are some docker commands that I use enough to want to share. I'm not saying they
are optimal (or modern), but I use the following commands regularly.

## Reset Local Docker

Sometimes, you just want to start fresh.

```bash
# Remove all containers
docker rm -f $(docker ps -aq)

# Remove all images
docker rmi -f $(docker images -aq)

# Remove all volumes
docker volume rm -f $(docker volume ls -aq)
```

## Starting `compose` Services

This will build along with start the containers. The `--build` flag will save you a 
command if you care about busting the build cache if there were changes (ie, code
changing that you then want updated in the container).

```bash
docker-compose up --build
```

## See container env vars

It's useful to see what environment variables are set in a container.

Quick and easy using a JSON array dump of env variables:

```bash
docker inspect 85331d6894cd | less

# then / to search for `"Env`
```

For `jq` fans:

```bash
docker inspect 85331d6894cd| jq '.[0].Config.Env'
```

If you have `rq` and want fancy columnar output to use with an `alias`:

```bash
docker inspect CONTAINER_ID_HERE |\
    jq '.[0].Config.Env' |\
    sed 's/^  "//' |\
    sed -E 's/",?$//' |\
    column -t -s=
```
