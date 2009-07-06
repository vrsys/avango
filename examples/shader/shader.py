import avango.osg
import avango.inspector
import avango.osg.simpleviewer

root = avango.osg.nodes.Group(Name="Root")

#vshader = avango.osg.nodes.Shader(Name="VertexShader",Type=0,ShaderSource="void main() { gl_Position=ftransform(); }")
vshader = avango.osg.nodes.Shader(Name="VertexShader",
                                  Type=avango.osg.shadertype.VERTEX,
                                  FileName="vshader.vert")

fshader = avango.osg.nodes.Shader(Name="FragmentShader",
                                  Type=avango.osg.shadertype.FRAGMENT,
                                  FileName="fshader.frag")

prog = avango.osg.nodes.Program(ShaderList=[vshader,fshader])

ss = avango.osg.nodes.StateSet(Program = prog)

# Important: Values have to be specified first !!!
ss.Uniforms.value = [avango.osg.nodes.Uniform(
                                              Values=[1.0,0.5,0.0,1.0],
                                              Type=avango.osg.uniformtype.FLOAT_VEC4,
                                              UniformName="diffuse"
                                              )
                     ]


sphere1 = avango.osg.nodes.Sphere(Name="RedSphere", StateSet=ss)

root.Children.value = [ sphere1]
inspector = avango.inspector.nodes.Inspector(Children=[root])
avango.osg.simpleviewer.run(root)
