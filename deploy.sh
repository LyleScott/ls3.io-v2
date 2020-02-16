set -ex

rm -rf public/

# Build site and create ./public
hugo

# Minimize CSS.
cp static/css/pure.css /tmp/combined.css
cat static/css/styles.css >> /tmp/combined.css
curl -X POST -s --data-urlencode 'input@/tmp/combined.css' https://cssminifier.com/raw \
    > public/css/styles.css

# Minimize HTML
html-minifier --input-dir ./public --output-dir ./public --collapse-whitespace --file-ext html

# Sync files to S3.
aws s3 sync \
    --delete \
    public/ s3://ls3.io

# Expire CDN assets so new content makes it out to browsers.
aws cloudfront \
    --profile personal \
    create-invalidation \
        --distribution-id E2RK2MVKQPV6WM \
        --paths '/*'
