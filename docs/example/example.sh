set -e

echo "Example usage of justomize with base.json and variant/overlay.json"
../../build/justomize --ignore-types base.json variant/overlay.json

echo "Example usage of justomize with output to a file"
../../build/justomize --ignore-types base.json variant/overlay.json -f output.json

echo "Example usage of justomize without ignoring type mismatches (this will throw an error due to the type mismatch in 'field_with_mismatched_type')"
../../build/justomize base.json variant/overlay.json || true