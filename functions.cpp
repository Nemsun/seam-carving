#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

// TODO Write this function
int energy(const Pixel *const*image, int col, int row, int width, int height)
{
  int energy = 0;
  int x = 0;
  int y = 0;
  int next_col = col + 1;
  int next_row = row + 1;
  int prev_col = col - 1;
  int prev_row = row - 1;

  if (next_col == width) {
    next_col = 0;
  }
  if (prev_col < 0) {
    prev_col = width - 1;
  }
  if (next_row == height) {
    next_row = 0;
  }
  if (prev_row < 0) {
    prev_row = height - 1;
  }

  int xr, xb, xg = 0;
  int yr, yb, yg = 0;
  xr = pow((image[next_col][row].r - image[prev_col][row].r),2);
  xg = pow((image[next_col][row].g - image[prev_col][row].g),2);
  xb = pow((image[next_col][row].b - image[prev_col][row].b),2);

  yr = pow((image[col][next_row].r - image[col][prev_row].r),2);
  yg = pow((image[col][next_row].g - image[col][prev_row].g),2);
  yb = pow((image[col][next_row].b - image[col][prev_row].b),2);
    
  x = xr + xg + xb;
  y = yr + yg + yb;
  energy = x + y;
  return energy;
}

int getVerticalSeam(const Pixel *const*image, int start_col, int width, int height, int* seam)
{

  int energy_l, energy_m, energy_r = 0;
  int t_energy = energy(image, start_col, 0, width, height);
  int energy_min = 0;

  if (width == 1) {
    for (int i = 0; i < height - 1; i++) {
      start_col = 0;
      int row = i;
      t_energy = t_energy + energy(image, start_col, row + 1, width, height);
      seam[i] = 0;
    }
    return t_energy;
  }

  for (int i = 0; i < height - 1; i++) {
    int row = i;
    seam[row] = start_col;
    if (start_col == 0) {
      energy_m = energy(image, start_col, row + 1, width, height);
      energy_r = energy(image, start_col + 1, row + 1, width, height);
      energy_min = 0;
      if (energy_m == energy_r) {
        energy_min = energy_m;
      } else if (energy_m > energy_r) {
        energy_min = energy_r;
        start_col = start_col + 1;
      } else {
        energy_min = energy_m;
      }
      t_energy = t_energy + energy_min;
    } else if (start_col == (width - 1)) {
      energy_m = energy(image, start_col, row + 1, width, height);
      energy_l = energy(image, start_col - 1, row + 1, width, height);
      if (energy_m == energy_l) {
        energy_min = energy_m;
      } else if (energy_m > energy_l) {
        energy_min = energy_l;
        start_col = start_col - 1;
      } else {
        energy_min = energy_m;
      }
      t_energy = t_energy + energy_min;
    } else {
      energy_m = energy(image, start_col, row + 1, width, height);
      energy_l = energy(image, start_col - 1, row + 1, width, height);
      energy_r = energy(image, start_col + 1, row + 1, width, height);
      if (energy_r == energy_l && energy_m == energy_r) {
        energy_min = energy_m;
      } else if (energy_l > energy_m && energy_r > energy_m) {
        energy_min = energy_m;
      } else if ((energy_m == energy_l && energy_m < energy_r) || (energy_m == energy_r && energy_m < energy_l)) {
        energy_min = energy_m;
      } else if (energy_r < energy_m && energy_r < energy_l) {
        energy_min = energy_r;
        start_col = start_col + 1;
      } else if (energy_r == energy_l && (energy_r < energy_m || energy_l < energy_m)) {
        energy_min = energy_r;
        start_col = start_col + 1;
      } else if (energy_l < energy_r && energy_l < energy_m) {
        energy_min = energy_l;
        start_col = start_col - 1;
      }
      t_energy = t_energy + energy_min;
    }
  }
  seam[height - 1] = start_col;
  return t_energy;
}

void removeVerticalSeam(Pixel **image, int& width, int height, int *verticalSeam)
{
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (j == verticalSeam[i]) {
        int k = j;
        while (k < (width - 1)) {
          image[k][i] = image[k + 1][i];
          k++;
        }
      }
    }
  }
  width = width - 1;
}

// TODO Write this function for extra credit
int getHorizontalSeam(const Pixel *const*image, int start_row, int width, int height, int* seam)
{
  if (image == nullptr) { ; }
  if (start_row == 0) { ; }
  if (width == 0) { ; } 
  if (height == 0) { ; }
  if (seam == nullptr) { ; }
  return 0;
}

// TODO Write this function for extra credit
void removeHorizontalSeam(Pixel **image, int width, int& height, int *horizontalSeam)
{
  if (image == nullptr) { ; }
  if (width == 0) { ; }
  if (height == 0) { ; }
  if (horizontalSeam == nullptr) { ; }

  
}

int *findMinVerticalSeam(const Pixel *const*image, int width, int height)
{
  // initialize minSeam and minDistance to seam starting at first col (index 0)
  int *minSeam = new int[height]{0};
  int minDist = getVerticalSeam(image, 0, width, height, minSeam);

  int *candidateSeam = new int[height]{0};
  int candidateDistance = -1; // invalid distance

  // start at second col (index 1) since we initialized with first col (index 0)
  for (int col = 1; col < width; ++col)
  {
    candidateDistance = getVerticalSeam(image, col, width, height, candidateSeam);

    if (candidateDistance < minDist)
    { // new min
      //  swap min & candidate
      minDist = candidateDistance;
      int* temp = candidateSeam;
      candidateSeam = minSeam;
      minSeam = temp;
    }
  }

  // clean up 
  delete [] candidateSeam;

  return minSeam;
}

int *findMinHorizontalSeam(const Pixel *const*image, int width, int height)
{
  // initialize minSeam and minDistance to seam starting at first row (index 0)
  int *minSeam = new int[width]{0};
  int minDistance = getHorizontalSeam(image, 0, width, height, minSeam);

  int *candidateSeam = new int[width]{0};
  int candidateDistance = -1; // invalid distance

  // start at second row (index 1) since we initialized with first row (index 0)
  for (int row = 1; row < height; ++row)
  {
    candidateDistance = getHorizontalSeam(image, row, width, height, candidateSeam);

    if (candidateDistance < minDistance)
    { // new minimum
      //  swap min and candidate seams
      minDistance = candidateDistance;
      int* temp = candidateSeam;
      candidateSeam = minSeam;
      minSeam = temp;
    }
  }

    // clean up 
  delete [] candidateSeam;

  return minSeam;
}

Pixel **createImage(int width, int height)
{
  cout << "Start createImage... " << endl;

  // Create a one dimensional array on the heap of pointers to Pixels
  //    that has width elements (i.e. the number of columns)
  Pixel **image = new Pixel *[width] {}; // initializes to nullptr

  for (int col = 0; col < width; ++col)
  { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    try
    {
      image[col] = new Pixel[height];
    }
    catch (std::bad_alloc &e)
    {
      // clean up already allocated arrays
      for (int i = 0; i < col; ++i)
      {
        delete[] image[i];
      }
      delete[] image;
      // rethrow
      throw e;
    }
  }

  // initialize cells
  // cout << "Initializing cells..." << endl;
  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      // cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = {0, 0, 0};
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel **image, int width)
{
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i = 0; i < width; ++i)
  {
    delete[] image[i];
  }
  delete[] image;
  image = nullptr;
  cout << "End deleteImage..." << endl;
}

bool isValidColor(int colorVal)
{
  if (colorVal < 0 || colorVal > 255)
  {
    return false;
  }
  return true;
}

Pixel ** loadImage(string filename, int &width, int &height)
{
  cout << "Start loadImage..." << endl;
  // remove
  ifstream ifs(filename);
  if (!ifs.is_open())
  {
    throw std::invalid_argument("Failed to open input file (" + filename + ")");
  }

  string type;
  ifs >> type; // should be P3
  if (toupper(type.at(0)) != 'P' || type.at(1) != '3')
  {
    throw std::domain_error("Not PPM type P3 (" + type + ")");
  }
  ifs >> width;
  // cout << "w and h: " << w << " " << h << endl;
  if (ifs.fail())
  {
    throw std::domain_error("Read non-integer value for width");
  }
  if (width <= 0)
  {
    ostringstream oss;
    oss << "Width in file must be greater than 0 (" << width << ")";
    throw std::domain_error(oss.str());
  }

  ifs >> height;
  if (ifs.fail())
  {
    cout << "Read non-integer value for height" << endl;
  }
  if (height <= 0)
  {
    ostringstream oss;
    oss << "Height in file must be greater than 0 (" << height << ")";
    throw std::domain_error(oss.str());
  }

  int colorMax = 0;
  ifs >> colorMax;
  if (ifs.fail())
  {
    throw std::domain_error("Read non-integer value for max color value");
  }
  if (colorMax != 255)
  {
    ostringstream oss;
    oss << "Max color value must be 255 (" << colorMax << ")";
    throw std::domain_error(oss.str());
  }

  // load image throws exceptions but we will let them pass through
  Pixel **image = createImage(width, height);

  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      // cout << "Pixel(" << col << ", " << row << ")" << endl;
      ifs >> image[col][row].r;
      if (ifs.fail() && !ifs.eof())
      { // bad input that is not caused by being at the end of the file
        throw std::domain_error("Read non-integer value for red");
      }
      if (!isValidColor(image[col][row].r))
      {
        ostringstream oss;
        oss << "Invalid color value for red (" << image[col][row].r << ")";
        throw std::domain_error(oss.str());
      }

      ifs >> image[col][row].g;
      if (ifs.fail() && !ifs.eof())
      { // bad input that is not caused by being at the end of the file
        throw std::domain_error("Read non-integer value for green");
      }
      if (!isValidColor(image[col][row].r))
      {
        ostringstream oss;
        oss << "Invalid color value for green (" << image[col][row].r + ")";
        throw std::domain_error(oss.str());
      }

      ifs >> image[col][row].b;
      if (ifs.fail() && !ifs.eof())
      { // bad input that is not caused by being at the end of the file
        throw std::domain_error("Read non-integer value for blue");
      }
      if (!isValidColor(image[col][row].r))
      {
        ostringstream oss;
        oss << "Invalid color value for blue (" << image[col][row].r + ")";
        throw std::domain_error(oss.str());
      }
    }
  }
  cout << "End loadImage..." << endl;
  return image;
}

void outputImage(string filename, const Pixel *const *image, int width, int height)
{
  cout << "Start outputImage..." << endl;
  // remove code
  // declare/define and open output file stream with filename
  ofstream ofs(filename);
  // ensure file is open
  if (!ofs.is_open())
  {
    throw std::invalid_argument("Error: failed to open output file - " + filename);
  }
  ofs << "P3" << endl;
  ofs << width << " " << height << endl;
  ofs << 255 << endl;
  for (int row = 0; row < height; ++row)
  {
    for (int col = 0; col < width; ++col)
    {
      ofs << image[col][row].r << " ";
      ofs << image[col][row].g << " ";
      ofs << image[col][row].b << " ";
    }
    ofs << endl;
  }
  cout << "End outputImage..." << endl;
}