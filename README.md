# Team Members
- **Josh Mundray** - 201696123
- **Aidan Evans** - 201759579
- **Krish Solanki** - 201736060
- **Jasem Abdulrasoul** - 201645943
- **Joseph Meredith** - 201718453

---

# Project Overview
This software is designed to provide a detailed analysis of water quality in specific areas. It offers comprehensive insights into various pollutants and their compliance levels through heatmaps and graphs.

# Setup Instructions
To set up and compile the project, follow these steps:
1. Create a build folder:
```bash
   mkdir build
   cd build
  ```
2. Generate the build files:
```bash
  cmake ..
```

3. Compile to executable:
```bash
  make
```
3. Run the application:
```bash
  ./waterQuality
```

### Key Features:
1. **Pollutant Overview Charts**
   - Enables users to query both locations and pollutants.
   - Visualises statistical data effectively.

2. **Heat Map**
   - Interactive 2D map of Yorkshire.
   - Displays colour-coded data points to represent measurement intensity, with red indicating the highest number of measurements.

3. **Compliance Dashboard**
   - Lists all pollutants, colour-coded based on their compliance with predefined thresholds.
   - Supports querying by pollutant and location.

---

# Dependencies
All dependencies are outlined in the `CMakeLists` file and listed below. They are part of the standard Qt framework and should be preinstalled on Red Hat machines. No additional requirements are needed.

### Required Qt Components:
- Qt Widgets
- Qt Charts
- Qt Core
- Qt Network
- Qt Quick
- Qt QML
- Qt Positioning
- Qt QuickWidgets
- Qt Concurrent

---

# File Structure
The code is modularised into several folders for clarity and maintainability:

1. **`<project>/utils`**
   - Contains utility classes that handle backend operations such as data parsing and analysis.

2. **`<project>/views`**
   - Includes all frontend views that users interact with.

3. **`<project>/styles` and `<project>/qml`**
   - Houses stylesheets and layout/design data for various components.

4. **`<project>/data`**
   - Stores CSV files (2021–2024) for quick access.
     
5. **`<project>/languages`**
   - Contains a language file with raw `.xml` files and compiled translations for internationalisation.

---

# Known Issues
Despite thorough testing, the following issues are known:

1. **Performance Delays**
   - The application temporarily freezes when rendering the data-page table.
   - Performance issues are also evident in the heat map, which stutters when interacted with due to the large volume of data points.

2. **Data Gaps**
   - Empty graphs or missing data may appear when selecting specific locations or pollutants.
   - This is due to inconsistent or inadequate data in the CSV files.
   - Dynamic location selection helps mitigate these issues, but some selectable locations still fail to render data on the graph.  
