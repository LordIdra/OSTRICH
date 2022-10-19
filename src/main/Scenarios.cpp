#include "Scenarios.h"
#include "bodies/Massive.h"
#include "rendering/shaders/Util.h"

#include <util/Log.h>
#include <main/Bodies.h>

#include <yaml-cpp/emitter.h>
#include <yaml-cpp/emittermanip.h>
#include <yaml-cpp/node/detail/iterator_fwd.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

#include <fstream>


namespace Scenarios {

    namespace {
        
        const float AMBIENT = 0.1;
        const float DIFFUSE = 0.8;
        const float SPECULAR = 0.3;
        const float SHINE = 32;

        auto GetString(YAML::Node node, const string &path1, const string &path2, const string &path3) -> string {
            if (!node[path3]) {
                Log(WARN, "Missing key at " + path1 + "." + path2 + "." + path3);
                return "";
            }

            return node[path3].as<string>();
        }

        auto SetString(YAML::Emitter &emitter, const string &key, const string &value) -> void {
            emitter << key << value;
        }

        auto GetDouble(YAML::Node node, const string &path1, const string &path2, const string &path3) -> double {
            if (!node[path3]) {
                Log(WARN, "Missing key at " + path1 + "." + path2 + "." + path3);
                return 0.0;
            }

            return node[path3].as<double>();
        }

        auto SetDouble(YAML::Emitter &emitter, const string &key, const double value) -> void {
            emitter << key << value;
        }

        auto GetVec3(YAML::Node node, const string &path1, const string &path2, const string &path3) -> vec3 {
            if (!node[path3]) {
                Log(WARN, "Missing key at " + path1 + "." + path2 + "." + path3);
                return vec3();
            }

            if (!node[path3].IsSequence()) {
                Log(WARN, "Expectec vec3 but got other type at " + path1 + "." + path2 + "." + path3);
                return vec3();
            }

            float x = node[path3][0].as<double>();
            float y = node[path3][1].as<double>();
            float z = node[path3][2].as<double>();

            return vec3(x, y, z);
        }

        auto SetVec3(YAML::Emitter &emitter, const string &key, const vec3 &value) -> void {
            emitter << key << YAML::BeginSeq << value.x << value.y << value.y << YAML::EndSeq;
        }

        auto GenerateMaterial(const vec3 color) -> Material {
            return Material{
                .ambient = AMBIENT * color, 
                .diffuse = DIFFUSE * color, 
                .specular = SPECULAR * color,
                .shine = SHINE};
        }

        auto LoadMassive(const string &id, const YAML::Node node, const string &path) -> Massive {
            string name = GetString(node, path, "massive", "name");
            vec3 color = GetVec3(node, path, "massive", "color");
            double radius = GetDouble(node, path, "massive", "radius");
            double mass = GetDouble(node, path, "massive", "mass");
            vec3 position = GetVec3(node, path, "massive", "position");
            vec3 velocity = GetVec3(node, path, "massive", "velocity");
            Material material = GenerateMaterial(color);
            return Massive(id, name, color, position, velocity, material, mass, radius);
        }

        auto LoadMassless(const string &id, const YAML::Node node, const string &path) -> Massless {
            string name = GetString(node, path, "massive", "name");
            vec3 color = GetVec3(node, path, "massive", "color");
            vec3 position = GetVec3(node, path, "massive", "position");
            vec3 velocity = GetVec3(node, path, "massive", "velocity");
            return Massless(id, name, color, position, velocity);
        }

        auto SaveMassive(const string &id, YAML::Emitter &emitter, const Massive &body) -> void {
            emitter << id;
            emitter << YAML::BeginMap;
            SetString(emitter, "name", body.GetName());
            SetVec3(emitter, "color", body.GetColor());
            SetDouble(emitter, "radius", body.GetRadius());
            SetDouble(emitter, "mass", body.GetMass());
            SetVec3(emitter, "position", body.GetPosition());
            SetVec3(emitter, "velocity", body.GetVelocity());
            emitter << YAML::EndMap;
        }
    }

    auto LoadScenario(const string &path) -> void{
        // Check if the file exists
        if (!FileExists(path)) {
            Log(WARN, "File not found at " + path);
            return;
        }

        YAML::Node scenario = YAML::LoadFile(path);
        YAML::Node massive = scenario["massive"];
        YAML::Node massless = scenario["massless"];
        
        // Check scenario file contains the 'massive' key
        if (!massive) {
            Log(WARN, "Missing key at " + path + "." + "massive");
            return;
        }

        // Check scenario file contains the 'massless' key
        if (!massless) {
            Log(WARN, "Missing key at " + path + "." + "massless");
            return;
        }

        // Load Massive bodies
        for (YAML::const_iterator i = massive.begin(); i != massive.end(); i++) {
            string id = i->first.as<string>();
            YAML::Node node = i->second;
            Bodies::AddBody(LoadMassive(id, node, path));
        }

        // Load massless bodies
        for (YAML::const_iterator i = massless.begin(); i != massless.end(); i++) {
            string id = i->first.as<string>();
            YAML::Node node = i->second;
            Bodies::AddBody(LoadMassless(id, node, path));
        }
    }

    auto SaveScenario(const string &path) -> void {
        // Check if the file already exists (we'd have to overwrite it if it does)
        if (FileExists(path)) {
            Log(WARN, "File already exists at " + path + " and would be overwritten");
            return;
        }

        YAML::Emitter scenario;
        scenario << YAML::BeginMap;
        
        // Save massive bodies
        scenario << YAML::Key << "massive" << YAML::Value;
        scenario << YAML::BeginMap;
        for (const auto &pair : Bodies::GetMassiveBodies()) {
            string id = pair.first;
            Massive body = pair.second;
            SaveMassive(id, scenario, body);
        }

        // Save massless bodies
        scenario << YAML::Key << "massless" << YAML::Value;
        scenario << YAML::BeginMap;
        for (const auto &pair : Bodies::GetMassiveBodies()) {
            string id = pair.first;
            Massive body = pair.second;
            SaveMassive(id, scenario, body);
        }
        scenario << YAML::EndMap;

        scenario << YAML::EndMap;

        // Save file
        std::ofstream file(path);
        file << scenario.c_str();
        file.close();
    }
}