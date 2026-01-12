#!/usr/bin/env python3
"""
Simple PNG rotation using PIL - run this manually to create rotated version
"""
try:
    from PIL import Image

    # Rotate minus.png by 90 degrees clockwise
    img = Image.open("assets/minus.png")
    rotated = img.rotate(-90, expand=True)  # Negative for clockwise
    rotated.save("assets/minus_rotated_90.png")
    print("Created assets/minus_rotated_90.png")

except ImportError:
    print("Error: PIL/Pillow not installed")
    print("Please install with: pip install Pillow")
    print("Then run this script again")
