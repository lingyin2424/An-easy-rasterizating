cmake_minimum_required(VERSION 3.20)
project(aSmallRender LANGUAGES CXX)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CUDA_STANDARD 14)


add_executable(aSmallRender main.cpp
        tgaimage.cpp
        math/Vec.cpp
        math/Mat.cpp
        Texture.cpp
        Thing.cpp
        Mesh.cpp
        Translate.cpp
        Camera.cpp
        Material.cpp
        Scene.cpp
        Render.cpp
        Face.cpp
        Shader/DefaultShader.cpp
        Shader/PhongShader.cpp
        Shader/PBRShader.cpp
        Shader/ShadowMapShader.cpp
        Shader/ShadowPbrShader.cpp
        Shader/PcssShader.cpp
        Shader/PcfShader.cpp
        Shader/VssmShader.cpp
        Shader/LoopSubdivision.cpp
 #       SafeQueue.cpp
#        ThreadPool.h
        Array2D.cpp
        Shader/VertexDecimation.cpp
        Shader/SimpleMesh.cpp
        #      Shader/VssmShader.cpp
  #      Shader/VsmShader.cpp
)
