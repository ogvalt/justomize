#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ---------------------------------------------------------------------------
// Recursive merge
//
// Rules:
//   - Both objects  → recurse; keys present in both are overridden by overlay
//   - Same non-object type or one is not an object → overlay wins
//   - Type mismatch → throw (unless ignore_types is true)
// ---------------------------------------------------------------------------
static json merge(const json& base, const json& overlay, bool ignore_types)
{
    // If both are objects, merge key by key
    if (base.is_object() && overlay.is_object()) {
        json result = base;
        for (auto& [key, overlay_val] : overlay.items()) {
            if (result.contains(key)) {
                result[key] = merge(result[key], overlay_val, ignore_types);
            } else {
                result[key] = overlay_val;
            }
        }
        return result;
    }

    // Type mismatch check (only when both values are "leaf" or one differs)
    if (base.type() != overlay.type()) {
        if (!ignore_types) {
            std::ostringstream msg;
            msg << "Type mismatch: base has type '"
                << base.type_name()
                << "' but overlay has type '"
                << overlay.type_name() << "'";
            throw std::runtime_error(msg.str());
        }
    }

    // Override base with overlay value
    return overlay;
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static json load_json(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    try {
        return json::parse(file);
    } catch (const json::parse_error& e) {
        throw std::runtime_error(
            "Failed to parse '" + path + "': " + e.what());
    }
}

static void print_usage(const char* prog)
{
    std::cerr
        << "Usage:\n"
        << "  " << prog << " [--ignore-types] <base.json> <overlay.json> [-f <output.json>]\n"
        << "\n"
        << "Options:\n"
        << "  --ignore-types   Override base field even when types differ\n"
        << "  -f <output.json> Write result to file instead of stdout\n";
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    bool        ignore_types = false;
    std::string base_path;
    std::string overlay_path;
    std::string output_path;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);

        if (arg == "--ignore-types") {
            ignore_types = true;
        } else if (arg == "-f") {
            if (i + 1 >= argc) {
                std::cerr << "Error: -f requires an output file path\n";
                print_usage(argv[0]);
                return 1;
            }
            output_path = argv[++i];
        } else if (base_path.empty()) {
            base_path = arg;
        } else if (overlay_path.empty()) {
            overlay_path = arg;
        } else {
            std::cerr << "Error: unexpected argument '" << arg << "'\n";
            print_usage(argv[0]);
            return 1;
        }
    }

    if (base_path.empty() || overlay_path.empty()) {
        std::cerr << "Error: both <base.json> and <overlay.json> are required\n";
        print_usage(argv[0]);
        return 1;
    }

    json base_json;
    json overlay_json;

    try {
        base_json    = load_json(base_path);
        overlay_json = load_json(overlay_path);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    json result;
    try {
        result = merge(base_json, overlay_json, ignore_types);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    const std::string output = result.dump(2) + "\n";

    if (output_path.empty()) {
        std::cout << output;
    } else {
        std::ofstream out_file(output_path);
        if (!out_file.is_open()) {
            std::cerr << "Error: cannot open output file: " << output_path << "\n";
            return 1;
        }
        out_file << output;
        if (!out_file) {
            std::cerr << "Error: failed to write to output file: " << output_path << "\n";
            return 1;
        }
    }

    return 0;
}
