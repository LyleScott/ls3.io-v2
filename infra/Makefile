default: update

stack_name ?= ls3io
env ?= dev

delete:
	aws cloudformation delete-stack \
		--stack-name $(stack_name)-$(env)

create:
	aws cloudformation create-stack \
		--stack-name $(stack_name)-${env} \
		--capabilities CAPABILITY_NAMED_IAM \
		--template-body file://formation.yml \
		--parameters file://parameters.$(env).json

update:
	aws cloudformation update-stack \
		--stack-name $(stack_name)-${env} \
		--capabilities CAPABILITY_NAMED_IAM \
		--template-body file://formation.yml \
		--parameters file://parameters.$(env).json
