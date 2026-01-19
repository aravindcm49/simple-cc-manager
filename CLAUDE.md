# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a bash-based CLI tool (`cc`) that enables switching between different AI API providers for Claude Code by setting environment variables. The tool supports Anthropic (default), GLM (Z.ai), MiniMax, and OpenRouter providers.

## Architecture

The entire project is a single bash script (`cc`) with the following structure:

- **`load_api_key()`** - Reads API keys from `apikeys.json` using Node.js for JSON parsing
- **Provider functions** (`provider_glm`, `provider_mmx`, `provider_dflt`, `provider_optr`) - Each clears existing ANTHROPIC_* variables, loads the appropriate API key, sets provider-specific environment variables, and optionally launches Claude Code
- **`clear_vars()`** - Unsets all ANTHROPIC_* environment variables
- **Variable setting functions** (`set_glm_vars`, `set_mmx_vars`, `set_dflt_vars`, `set_optr_vars`) - Define provider-specific configurations (BASE_URL, model mappings, timeouts)
- **`fetch_optr_models()`** - Fetches tool-use compatible models from OpenRouter API
- **`select_optr_model()`** - Interactive model selection using fzf (falls back to numbered list if fzf unavailable)

Each provider exports different environment variables that Claude Code reads:
- `ANTHROPIC_BASE_URL` - API endpoint
- `ANTHROPIC_AUTH_TOKEN` or `ANTHROPIC_API_KEY` - Authentication
- `ANTHROPIC_DEFAULT_*_MODEL` - Model mappings for Haiku/Sonnet/Opus tiers
- `API_TIMEOUT_MS` - Request timeout (3000000ms for all providers)
- `CLAUDE_CODE_DISABLE_NONESSENTIAL_TRAFFIC` - MiniMax-specific flag
- `ANTHROPIC_API_KEY` - Must be explicitly empty ("") for OpenRouter to prevent authentication conflicts

## Development

### Setup

```bash
# Copy sample API keys file
cp apikeys.json.sample apikeys.json
# Edit apikeys.json with actual keys
```

### Docker Sandbox with Providers (dcc)

The `dcc` command runs Docker sandbox with provider-specific configurations and preset environment variables:

```bash
# Run Docker sandbox with different providers
dcc glm    # Use GLM provider
dcc mmx    # Use MiniMax provider
dcc dflt   # Use Anthropic (default) provider
```

**What `dcc` does:**
1. Loads the selected provider's API key from `apikeys.json`
2. Sets provider-specific environment variables (BASE_URL, model mappings)
3. Runs: `docker sandbox run -e <preset_vars>... claude`

**Preset environment variables** (editable in `dcc` script):
- `NODE_ENV=development`
- `DATABASE_URL=postgresql://localhost/myapp_dev`
- `REDIS_URL=redis://localhost:6379`
- `LOG_LEVEL=debug`

To customize these variables, edit the `PRESET_ENV_VARS` array in the `dcc` script.

### Testing Changes

Since this is a bash script, test changes by:

```bash
# Source the script to test functions in current shell
source ./ccm

# Test individual functions
provider_glm --no-run
provider_mmx --no-run
provider_clr
```

### Adding a New Provider

To add a new AI provider:

1. Add the API key to `apikeys.json`
2. Create a `set_<provider>_vars()` function to set provider-specific environment variables
3. Create a `provider_<provider>()` function following the existing pattern:
    - Call `clear_vars`
    - Load API key with `load_api_key "<provider>"`
    - Export `ANTHROPIC_AUTH_TOKEN` or `ANTHROPIC_API_KEY`
    - Call the provider's vars function
    - Handle `--no-run` flag
4. Add the provider name to the `case` statement in `main()`
5. Update the `usage()` function

**Note for OpenRouter:** When adding OpenRouter or similar proxies, ensure `ANTHROPIC_API_KEY` is explicitly set to an empty string to prevent authentication conflicts with Anthropic servers.

