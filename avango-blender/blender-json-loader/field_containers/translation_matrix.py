import avango
import avango.gua
import avango.script


class TranslationMatrix(avango.script.Script):

    # Input
    Vector = avango.gua.SFVec3()
    # Output
    Matrix = avango.gua.SFMatrix4()

    def __init__(self):
        self.super(TranslationMatrix).__init__()
        self.Name.value = "RotationMatrix"
        self.Vector.value = avango.gua.Vec3(0.0, 0.0, 0.0)

    def constructor(self, json, app):
        self.Name.value = json["name"]

        values = json["values"]
        self.Vector.value = avango.gua.Vec3(
            values["Vector"][0],
            values["Vector"][1],
            values["Vector"][2],
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
        self.Matrix.value = avango.gua.make_trans_mat(self.Vector.value)
