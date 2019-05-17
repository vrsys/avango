import avango
import avango.script

class Picker(avango.script.Script):
    SceneGraph = avango.gua.SFSceneGraph()
    Ray        = avango.gua.SFRayNode()
    Options    = avango.SFInt()
    BlackList  = []
    WhiteList  = []
    Results    = avango.gua.MFPickResult()

    def __init__(self):
        self.super(Picker).__init__()
        self.always_evaluate(True)

        self.SceneGraph.value = avango.gua.nodes.SceneGraph()
        self.Ray.value  = avango.gua.nodes.RayNode()
        self.Options.value = avango.gua.PickingOptions.GET_TEXTURE_COORDS \
                             | avango.gua.PickingOptions.GET_WORLD_NORMALS \
                             | avango.gua.PickingOptions.INTERPOLATE_NORMALS \
                             | avango.gua.PickingOptions.PICK_ONLY_FIRST_FACE

    def evaluate(self):
        results = self.SceneGraph.value.ray_test(self.Ray.value,
                                                 self.Options.value,
                                                 self.BlackList,
                                                 self.WhiteList)
        if len(results.value) > 0:
            self.Results.value = results.value
            
