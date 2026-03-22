# epita-ocr

OCR engine built from scratch in C. Takes an image as input, extracts text from it, and solves the sudoku puzzle it contains — no external ML libraries.

## What it does

1. **Preprocessing** — converts the image to grayscale, applies noise reduction and binarization
2. **Detection** — locates characters and words in the image using connected-component analysis
3. **Recognition** — classifies each character through a custom convolutional neural network trained on handwritten digits and letters
4. **Puzzle solving** — extracts the sudoku grid from the recognized characters and solves it

## Architecture

```
Projet/
├── FichierC/          # Image processing and OCR pipeline
│   ├── to_black_and_white.c   # Grayscale + binarization
│   ├── detecte.c              # Character detection
│   ├── extractletter.c        # Letter extraction
│   ├── readletter1.c          # Letter recognition
│   ├── detectermot.c          # Word detection
│   ├── solver.c               # Sudoku solver
│   └── net.c                  # Neural network interface
├── FichierCIA/        # Neural network
│   ├── cnn.c                  # Convolutional neural network
│   └── to28.c                 # Image normalization (28x28)
└── generateur_grille/ # Grid and test image generation
```

## Build

```bash
cd Projet
make
./main <image>
```

**Dependencies:** SDL2, SDL2_image, GTK+-3.0

## Team

Ilann Oller · Théo Hervé · Baptiste Mathieu · Lenny Taurines

EPITA — 3rd year project (S3)
