import avango
import avango.gua
import avango.script
import math


def function_add(first, second):
    return first + second


def function_sub(first, second):
    return first - second


def function_mult(first, second):
    return first * second


def function_div(first, second):
    return first / second


def function_pow(first, second):
    return math.pow(first, second)


def function_mod(first, second):
    return first % second


def function_root(first, second):
    return math.pow(first, 1.0/second)


def function_sin(first, second):
    return math.sin(first) * second


class FloatMath(avango.script.Script):

    # Input
    First = avango.SFFloat()
    Second = avango.SFFloat()
    # Output
    Result = avango.SFFloat()

    operator_dict = {
        'ADD': function_add,
        'SUBTRACT': function_sub,
        'MULTIPLY': function_mult,
        'DIVIDE': function_div,
        'POWER': function_pow,
        'MODULO': function_mod,
        'ROOT': function_root,
        'SINUS': function_sin
    }

    def __init__(self):
        self.super(FloatMath).__init__()
        self.Name.value = "FloatMath"
        self.First.value = 0.0
        self.Second.value = 0.0

    def constructor(self, json, app):
        self.Name.value = json["name"].replace('.', '_')

        values = json["values"]
        self.First.value = values["First"]
        self.Second.value = values["Second"]

        app.add_field_container(self)

        for connection in json["field_connections"]:
            app.plan_field_connection(
                self.Name.value,
                connection["from_field"],
                connection["to_node"],
                connection["to_field"]
            )

        self.operator = FloatMath.operator_dict[json["operator"]]

    def set_field_value(self, field_name, value):
        field = self.get_field(field_name)
        field.value = value

    def evaluate(self):
        self.Result.value = self.operator(self.First.value, self.Second.value)
