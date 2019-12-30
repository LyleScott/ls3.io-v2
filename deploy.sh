set -ex

# Build site and create ./public
hugo

# Minimize CSS.
#cp static/css/bootstrap.min.css static/css/styles.css
#cat static/css/custom.css >> /tmp/combined.css
#curl -X POST -s --data-urlencode 'input@/tmp/combined.css' https://cssminifier.com/raw \
#    >> static/css/styles.css
html-minifier --input-dir ./public --output-dir ./public --collapse-whitespace --file-ext html

# Sync files to S3.
aws s3 sync \
    --quiet \
    --delete \
    public/ s3://ls3.io
