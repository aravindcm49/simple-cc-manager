# CC - AI Provider Switcher for Claude Code

A CLI tool to switch between different AI API providers for Claude Code.

## Features

- Switch between Anthropic (default), GLM, and MiniMax providers
- Auto-launch Claude Code after switching
- Set environment variables easily with one command
- Support for `--no-run` flag to set variables without launching

## Installation

1. Clone or download this repository
2. Navigate to the repository directory: `cd path/to/ccm`
3. Make the script executable (if needed): `chmod +x cc`

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

```bash
cc dflt            # Switch to Anthropic and run claude
cc dflt --no-run   # Switch to Anthropic without running
cc glm --no-run    # Switch to GLM without running
cc mmx             # Switch to MiniMax and run claude
cc clr             # Clear all ANTHROPIC_* variables
```

## Available Providers

| Command | Provider | Base URL |
|---------|----------|----------|
| `dflt` | Anthropic (default) | https://api.anthropic.com |
| `glm` | GLM (Z.ai) | https://api.z.ai/api/anthropic |
| `mmx` | MiniMax | https://api.minimax.io/anthropic |

## Notes

- When using `cc <provider>` without `--no-run`, the tool will display the switched variables, wait 500ms, clear the screen, and automatically launch Claude Code
- When using `--no-run`, variables are set but Claude Code is not launched (use `claude` command manually)
- The `clr` command clears all ANTHROPIC_* variables without launching anything

## License

MIT License
