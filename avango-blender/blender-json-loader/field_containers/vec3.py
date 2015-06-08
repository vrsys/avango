import avango
import avango.gua
import avango.script


class Vec3(avango.script.Script):

    # Input
    X = avango.SFFloat()
    Y = avango.SFFloat()
    Z = avango.SFFloat()

    # Output
    Vector = avango.gua.SFVec3()

    def __init__(self):
        self.super(Vec3).__init__()

        self.Name.value = "Vec3"

        self.X.value = 0.0
        self.Y.value = 0.0
        self.Z.value = 0.0
        self.Vector.value = avango.gua.Vec3(0.0, 0.0, 0.0)

    def constructor(self, json, app):
        self.Name.value = json["name"]

        values = json["values"]
        self.X.value = values["X"]
        self.Y.value = values["Y"]
        self.Z.value = values["Z"]

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
        self.Vector.value = avango.gua.Vec3(
            self.X.value,
            self.Y.value,
            self.Z.value,
        )
