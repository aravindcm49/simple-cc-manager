# CC - AI Provider Switcher for Claude Code

A CLI tool to switch between different AI API providers for Claude Code.

## Features

- Switch between Anthropic (default), GLM, MiniMax, and OpenRouter providers
- Auto-launch Claude Code after switching
- Set environment variables easily with one command
- Support for `--no-run` flag to set variables without launching

## Installation

1. Clone or download this repository
2. Navigate to the repository directory: `cd path/to/ccm`
 3. Make the script executable (if needed): `chmod +x cc`
4. Install `fzf` (required for OpenRouter's interactive model selection):
   ```bash
   brew install fzf
   ```

### Add to PATH

Choose one of the following methods:

**Option 1 - Add current directory to PATH (temporary, lasts for current session):**
```bash
export PATH="$(pwd):$PATH"
```

**Option 2 - Add specific directory to PATH (permanent):**
```bash
# For bash
echo 'export PATH="/path/to/ccm:$PATH"' >> ~/.bashrc

# For zsh
echo 'export PATH="/path/to/ccm:$PATH"' >> ~/.zshrc

# Source the file or restart terminal
source ~/.bashrc  # or source ~/.zshrc
```

## API Key Setup

1. Copy the sample file:
```bash
cp apikeys.json.sample apikeys.json
```

2. Edit `apikeys.json` and add your actual API keys

3. Keep `apikeys.json` private (it's already in `.gitignore`)

## Usage

### Normal Mode (auto-launches Claude)
```bash
cc dflt            # Switch to Anthropic and run claude
cc glm             # Switch to GLM and run claude
cc mmx             # Switch to MiniMax and run claude
cc optr            # Switch to OpenRouter (interactive model selection)
cc optr -m <model> # Switch to OpenRouter with specific model
cc chk             # Check current provider status
```

### With Claude Flags
```bash
cc dflt -c                    # Switch to Anthropic, run with --continue
cc glm -r session-123         # Switch to GLM, resume specific session
cc mmx --continue             # Switch to MiniMax, continue conversation
```

### Manual Mode (set variables without launching)
```bash
# IMPORTANT: When using --no-run or clr, you must SOURCE the script
source cc glm --no-run        # Set GLM variables in current shell
source cc clr                 # Clear variables from current shell
. cc dflt --no-run            # Alternative syntax (same as source)
```

After setting variables with `source`, you can then run `claude` manually and it will pick up the provider configuration.

## Available Providers

| Command | Provider | Base URL |
|---------|----------|----------|
| `dflt` | Anthropic (default) | https://api.anthropic.com |
| `glm` | GLM (Z.ai) | https://api.z.ai/api/anthropic |
| `mmx` | MiniMax | https://api.minimax.io/anthropic |
| `optr` | OpenRouter | https://openrouter.ai/api |

## Important Notes

### How Environment Variables Work
- **Normal commands** (`cc dflt`, `cc glm`, etc.) run in a subshell and auto-launch `claude` within that subshell, so everything works.
- **Manual mode** (`--no-run`) and **clear command** (`clr`) need to affect your current shell, so you must `source` them.
- **Check command** (`chk`) only reads variables and works fine either way.

### When to Use Source
```bash
# Use source when you want to set/clear variables but NOT launch claude
source cc glm --no-run   # Set vars, then run 'claude' manually
source cc clr            # Clear vars from your shell

# No source needed when auto-launching claude
cc glm                   # This works fine
cc dflt -c               # This works fine
```

### Behavior
- When using `cc <provider>` without `--no-run`, the tool will display the switched variables, wait 500ms, clear the screen, and automatically launch Claude Code
- When using `source cc <provider> --no-run`, variables persist in your current shell for manual `claude` invocation
- The `chk` command shows which provider is currently active based on `ANTHROPIC_BASE_URL`

### OpenRouter Provider Notes
- The OpenRouter provider (`optr`) requires tool-use compatible models
- Model selection is interactive using `fzf` when available, otherwise falls back to numbered list
- `ANTHROPIC_API_KEY` is explicitly set to empty string to prevent 405 authentication errors

## License

MIT License
