from examples_common.GuaVE import GuaVE
import avango.gua


class Application:

    def __init__(self):
        self.viewer = avango.gua.nodes.Viewer(Name='Viewer')
        self.scenegraph = avango.gua.nodes.SceneGraph(Name='SceneGraph')
        self.window = None
        self.screen = None
        self.camera = None
        self.field_containers = {}
        self.planned_field_connections = []
        self.root = avango.gua.nodes.TransformNode(
            Name="Custom_Root",
        )
        self.scenegraph.Root.value.Children.value.append(self.root)

    def run(self):
        guaVE = GuaVE()
        guaVE.start(locals(), globals(), show_banner=False)
        tmp = self.camera.EnableFrustumCulling.value
        self.camera.EnableFrustumCulling.value = False
        self.viewer.frame()
        self.camera.EnableFrustumCulling.value = tmp
        self.viewer.run()

    def basic_setup(self):

        size = avango.gua.Vec2ui(1600, 900)

        # self.camera = avango.gua.nodes.CameraNode(
        #   LeftScreenPath = "/screen",
        #   SceneGraph = "SceneGraph",
        #   Resolution = size,
        #   OutputWindowName = "window",
        #   Transform = avango.gua.make_trans_mat(0.0, 0.0, 3.5)
        # )

        # self.screen = avango.gua.nodes.ScreenNode(
        #   Name = "screen",
        #   Width = 2,
        #   Height = 1.5,
        #   Children = [self.camera],
        #   Transform = avango.gua.make_trans_mat(0.0, 0.0, 15.0)
        # )
        self.camera.LeftScreenPath.value = self.screen.Path.value

        self.scenegraph.Root.value.Children.value.append(self.screen)

        self.window = avango.gua.nodes.GlfwWindow(
            Size=size,
            LeftResolution=size,
        )
        avango.gua.register_window("window", self.window)

        self.viewer.SceneGraphs.value = [self.scenegraph]
        self.viewer.Windows.value = [self.window]

    def add_field_container(self, field_container):
        self.field_containers[field_container.Name.value] = field_container

    def add_field_connection(
            self,
            from_container_name,
            from_field_name,
            to_container_name,
            to_field_name):

        to_field =\
            self.field_containers[to_container_name].get_field(to_field_name)
        from_field = self.field_containers[from_container_name].\
            get_field(from_field_name)
        print(("add field connection", to_field_name))
        to_field.connect_from(from_field)

    def plan_field_connection(
            self,
            from_container_name,
            from_field_name,
            to_container_name,
            to_field_name):

        from_container_name = from_container_name.replace('.', '_')
        to_container_name = to_container_name.replace('.', '_')
        self.planned_field_connections.append((
            from_container_name,
            from_field_name,
            to_container_name,
            to_field_name
        ))

    def apply_field_connections(self):
        for fc in self.planned_field_connections:
            self.add_field_connection(fc[0], fc[1], fc[2], fc[3])
        self.planned_field_connections = []

    def set_camera(self, camera):
        self.camera = camera
