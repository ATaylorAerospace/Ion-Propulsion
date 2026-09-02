# docs

| File | Purpose |
|------|---------|
| `geosats.png` | Repository banner shown at the top of the main README (2480 x 1380). |
| `geosats.svg` | Editable source of the banner. Every number on it comes from the code and the test suites. |
| `archive/geosats_2025_collage.png` | The original 2025 banner, kept for reference. |

## Regenerating the banner

Edit `geosats.svg`, then rasterise it with any headless Chromium:

```bash
chrome --headless --no-sandbox --hide-scrollbars --window-size=2480,1380 \
      --force-device-scale-factor=1 --screenshot=docs/geosats.png docs/geosats.svg
```

The banner uses the DejaVu Sans and DejaVu Sans Mono fonts (Liberation as fallback), the same family as the Thermal-Agent banner, so the two repositories render as a matched series.

Author: A Taylor
