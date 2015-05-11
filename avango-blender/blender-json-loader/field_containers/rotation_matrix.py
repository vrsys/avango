import avango
import avango.gua
import avango.script


class RotationMatrix(avango.script.Script):

    # Input
    Angle = avango.SFFloat()
    Axis = avango.gua.SFVec3()

    # Output
    Matrix = avango.gua.SFMatrix4()

    def __init__(self):
        self.super(RotationMatrix).__init__()
        self.Name.value = "RotationMatrix"
        self.Angle.value = 0
        self.Axis.value = avango.gua.Vec3(0.0, 1.0, 0.0)

    def constructor(self, json, app):
        self.Name.value = json["name"].replace('.', '_')

        values = json["values"]
        self.Angle.value = values["Angle"]
        self.Axis.value = avango.gua.Vec3(
            values["Axis"][0],
            values["Axis"][1],
            values["Axis"][2],
        )

        app.add_field_container(self)

        for connection in json["field_connections"]:
            app.plan_field_connection(
                self.Name.value,
                connection["from_field"],
                connection["to_node"],
                connection["to_field"],
            )

    def set_field_value(self, field_name, value):
        field = self.get_field(field_name)
        field.value = value

    def evaluate(self):
        self.Matrix.value =\
            avango.gua.make_rot_mat(self.Angle.value, self.Axis.value)
