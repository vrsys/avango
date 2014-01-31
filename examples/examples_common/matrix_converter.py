import avango.gua
import avango.osg

def convert_osg_to_gua(osg_mat):
  gua_mat = avango.gua.make_identity_mat()
  for row in range(0, 4):
    for col in range(0, 4):
      gua_mat.set_element(col, row, osg_mat.get_element(row, col))
      
  return gua_mat
  
def convert_gua_to_osg(gua_mat):
  gua_mat = avango.osg.make_identity_mat()
  for row in range(0, 4):
    for col in range(0, 4):
      osg_mat.set_element(col, row, gua_mat.get_element(row, col))
      
  return osg_mat
