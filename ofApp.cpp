#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 180, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	vector<string> word_list = {
		"A", "B", "C",
		"1", "2", "3",
		"X", "Y", "Z"
	};

	vector<glm::vec2> base_location_list = {
		glm::vec2(this->font.stringWidth(word_list[0]) * 0.5 - 150, -150), glm::vec2(this->font.stringWidth(word_list[0]) * 0.5, -150), glm::vec2(this->font.stringWidth(word_list[0]) * 0.5 + 150, -150),
		glm::vec2(this->font.stringWidth(word_list[0]) * 0.5 - 150, 60),   glm::vec2(this->font.stringWidth(word_list[0]) * 0.5, 60),   glm::vec2(this->font.stringWidth(word_list[0]) * 0.5 + 150, 60),
		glm::vec2(this->font.stringWidth(word_list[0]) * 0.5 - 150, 270),  glm::vec2(this->font.stringWidth(word_list[0]) * 0.5, 270),  glm::vec2(this->font.stringWidth(word_list[0]) * 0.5 + 150, 270)
	}; 

	for (int i = 0; i < word_list.size(); i++) {

		auto word = word_list[i];
		auto base_location = base_location_list[i];
		base_location += glm::vec2(this->font.stringWidth(word_list[i]) * 0.5, this->font.stringHeight(word_list[i]) * 0.5);

		int sample_count = 120;
		vector<ofPath> word_path = this->font.getStringAsPoints(word, true, false);
		for (int word_index = 0; word_index < word_path.size(); word_index++) {

			vector<ofPolyline> outline = word_path[word_index].getOutline();
			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

				outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
				vector<glm::vec3> vertices = outline[outline_index].getVertices();
				vector<glm::vec2> mesh_vertices;
				vector<glm::highp_mat4x4> rotate_vertices;
				
				for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

					auto noise_value = ofNoise(
						(base_location.x + vertices[vertices_index].x) * 0.001,
						(base_location.y + vertices[vertices_index].y) * 0.001 + ofGetFrameNum() * 0.008);
					float param = 0.5;
					if (noise_value < 0.3) { param = ofMap(noise_value, 0, 0.3, 0, 0.5); }
					if (noise_value > 0.7) { param = ofMap(noise_value, 0.7, 1, 0.5, 1); }

					auto rotation = glm::rotate(glm::mat4(), ofMap(param, 0, 1, -PI * 3, PI * 3), glm::vec3(0, 1, 0));
					auto location = vertices[vertices_index] - glm::vec2(this->font.stringWidth(word_list[i]) * 0.5, this->font.stringHeight(word_list[i]) * 0.5);
					
					mesh_vertices.push_back(location);
					rotate_vertices.push_back(rotation);
				}

				ofMesh face, line;
				line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
				for (int k = 0; k < mesh_vertices.size(); k++) {

					face.addVertex(glm::vec4(base_location, 0, 0) + glm::vec4(mesh_vertices[k], -15, 0) * rotate_vertices[k]);
					face.addVertex(glm::vec4(base_location, 0, 0) + glm::vec4(mesh_vertices[k], 15, 0) * rotate_vertices[k]);

					line.addVertex(glm::vec4(base_location, 0, 0) + glm::vec4(mesh_vertices[k], -15, 0) * rotate_vertices[k]);
					line.addVertex(glm::vec4(base_location, 0, 0) + glm::vec4(mesh_vertices[k], 15, 0) * rotate_vertices[k]);

					if (k > 0) {

						face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 4);
						face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);

						line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 3);
						line.addIndex(line.getNumVertices() - 2); line.addIndex(line.getNumVertices() - 4);
					}
				}

				face.addIndex(face.getNumVertices() - 1); face.addIndex(0); face.addIndex(1);
				face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(2);

				line.addIndex(line.getNumVertices() - 1); line.addIndex(1);
				line.addIndex(line.getNumVertices() - 2); line.addIndex(0);

				ofSetColor(39);
				face.draw();

				ofSetColor(139);
				line.drawWireframe();
			}
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}