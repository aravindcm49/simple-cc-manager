#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
API_KEYS_FILE="$SCRIPT_DIR/apikeys.json"

UNSET_VARS=(
    "ANTHROPIC_AUTH_TOKEN"
    "ANTHROPIC_API_KEY"
    "ANTHROPIC_BASE_URL"
    "API_TIMEOUT_MS"
    "CLAUDE_CODE_DISABLE_NONESSENTIAL_TRAFFIC"
    "ANTHROPIC_MODEL"
    "ANTHROPIC_SMALL_FAST_MODEL"
    "ANTHROPIC_DEFAULT_HAIKU_MODEL"
    "ANTHROPIC_DEFAULT_SONNET_MODEL"
    "ANTHROPIC_DEFAULT_OPUS_MODEL"
)

load_api_key() {
    local provider="$1"
    if [ ! -f "$API_KEYS_FILE" ]; then
        echo "Error: API keys file not found: $API_KEYS_FILE" >&2
        exit 1
    fi

    local key
    key=$(node -e "
        const fs = require('fs');
        const data = JSON.parse(fs.readFileSync('$API_KEYS_FILE', 'utf8'));
        const key = data['$provider'];
        if (!key) {
            console.error('Provider not found: $provider');
            process.exit(1);
        }
        console.log(key);
    " 2>&1) || {
        echo "Error: $key" >&2
        exit 1
    }

    echo "$key"
}

clear_vars() {
    for var in "${UNSET_VARS[@]}"; do
        unset "$var" 2>/dev/null || true
    done
}

set_glm_vars() {
    export ANTHROPIC_BASE_URL="https://api.z.ai/api/anthropic"
    export API_TIMEOUT_MS="3000000"
    export ANTHROPIC_DEFAULT_HAIKU_MODEL="glm-4.5-air"
    export ANTHROPIC_DEFAULT_SONNET_MODEL="glm-4.7"
    export ANTHROPIC_DEFAULT_OPUS_MODEL="glm-4.7"
}

set_mmx_vars() {
    export ANTHROPIC_BASE_URL="https://api.minimax.io/anthropic"
    export API_TIMEOUT_MS="3000000"
    export CLAUDE_CODE_DISABLE_NONESSENTIAL_TRAFFIC="1"
    export ANTHROPIC_MODEL="MiniMax-M2.1"
    export ANTHROPIC_SMALL_FAST_MODEL="MiniMax-M2.1"
    export ANTHROPIC_DEFAULT_HAIKU_MODEL="MiniMax-M2.1"
    export ANTHROPIC_DEFAULT_SONNET_MODEL="MiniMax-M2.1"
    export ANTHROPIC_DEFAULT_OPUS_MODEL="MiniMax-M2.1"
}

set_dflt_vars() {
    export ANTHROPIC_BASE_URL="https://api.anthropic.com"
    export API_TIMEOUT_MS="3000000"
}

print_vars() {
    echo ""
    echo "Current ANTHROPIC_* variables:"
    env | grep "^ANTHROPIC_" | cut -d= -f1 | sort | sed 's/^/  /'
    echo ""
}

run_claude() {
    if command -v claude &>/dev/null; then
        claude
    else
        echo "Run 'claude' to start Claude Code."
    fi
}

provider_glm() {
    local no_run="${1:-}"
    clear_vars
    local api_key
    api_key=$(load_api_key "glm")
    export ANTHROPIC_AUTH_TOKEN="$api_key"
    set_glm_vars
    echo "✓ Switched to GLM"
    print_vars
    if [ "$no_run" != "--no-run" ]; then
        sleep 0.5
        clear
        run_claude
    else
        echo "Variables set without running claude (use 'claude' to launch)"
    fi
}

provider_mmx() {
    local no_run="${1:-}"
    clear_vars
    local api_key
    api_key=$(load_api_key "minimax")
    export ANTHROPIC_AUTH_TOKEN="$api_key"
    set_mmx_vars
    echo "✓ Switched to MiniMax"
    print_vars
    if [ "$no_run" != "--no-run" ]; then
        sleep 0.5
        clear
        run_claude
    else
        echo "Variables set without running claude (use 'claude' to launch)"
    fi
}

provider_dflt() {
    local no_run="${1:-}"
    clear_vars
    local api_key
    api_key=$(load_api_key "anthropic")
    export ANTHROPIC_API_KEY="$api_key"
    set_dflt_vars
    echo "✓ Switched to Anthropic (default)"
    print_vars
    if [ "$no_run" != "--no-run" ]; then
        sleep 0.5
        clear
        run_claude
    else
        echo "Variables set without running claude (use 'claude' to launch)"
    fi
}

provider_clr() {
    clear_vars
    echo "✓ Cleared all provider variables"
}

usage() {
    echo "Usage: cc <command> [--no-run]"
    echo ""
    echo "Commands:"
    echo "  dflt    - Switch to Anthropic (default) provider"
    echo "  glm     - Switch to GLM provider"
    echo "  mmx     - Switch to MiniMax provider"
    echo "  clr     - Clear all provider variables"
    echo ""
    echo "Options:"
    echo "  --no-run - Don't launch claude after switching"
    echo ""
    echo "Examples:"
    echo "  cc dflt        # Switch to Anthropic and run claude"
    echo "  cc dflt --no-run  # Switch to Anthropic without running"
    echo "  cc glm --no-run   # Switch to GLM without running"
    echo "  cc mmx         # Switch to MiniMax and run claude"
    echo "  cc clr         # Clear all ANTHROPIC_* variables"
}

main() {
    if [ $# -eq 0 ]; then
        usage
        exit 0
    fi

    local cmd="$1"
    local no_run=""

    if [ $# -ge 2 ] && [ "$2" = "--no-run" ]; then
        no_run="--no-run"
    fi

    case "$cmd" in
        glm|mmx|clr|dflt)
            "provider_$cmd" "$no_run"
            ;;
        -h|--help|help)
            usage
            ;;
        *)
            echo "Unknown command: $cmd" >&2
            usage >&2
            exit 1
            ;;
    esac
}

main "$@"
