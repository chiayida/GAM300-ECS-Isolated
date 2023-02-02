/******************************************************************************/
/*!
\file       ModelManager.cpp
\author     Chia Yi Da
\param      c.yida
\param      GAM 350
\date       16 January 2023
\brief
  This file contains the implementations of Model class.

  It also has a function which loads .obj file given the model name.

  Deprecated code for now. Not in use and there are probably minor issues with it.

  Copyright (C) 2023 DigiPen Institure of Technology.
  Reproduction or disclosure of this file or its contents
  without the prior written consent of DigiPen Institute of
  Technology is prohibited.
*/
/******************************************************************************/
// Standard Includes
#include "include/Graphics/ModelManager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace Engine
{
    Model LoadModelFile(std::string name_)
    {
        // File path
        std::string path = "Assets/" + name_ + ".obj";

        Model tempModel;

        // Open file and read
        std::ifstream ifs{ path.c_str(), std::ios::in };
        if (!ifs)
        {
            std::cout << "ERROR: Unable to open .obj file: " << path << std::endl;

            return tempModel;
        }
        else
        {
            // Go back to the start of the file.
            ifs.seekg(0, std::ios::beg);

            std::string line;

            // Get new line every loop
            while (std::getline(ifs, line))
            {
                std::string type;
                std::stringstream ss;
                ss.str(line);

                // First string is type
                ss >> type;

                switch (type[0])
                {
                    // Vertices
                case 'v':
                {
                    float tempVertex;

                    while (!ss.eof())
                    {
                        ss >> tempVertex;
                        tempModel.vertices.push_back(tempVertex);
                    }
                    break;
                }
                // Indices
                case 'f':
                {
                    unsigned int tempIndex;

                    while (!ss.eof())
                    {
                        ss >> tempIndex;

                        // -1 for indices as vertices index starts from 0 in container
                        tempModel.indices.push_back(tempIndex - 1);
                    }
                    break;
                }
                default:
                    break;
                }
            }

            return tempModel;
        }
    }
}