/*!
 * \file KWUtilYaml.h
 * \author Konrad Werys
 * \date 2018/11/16
 */


#ifndef KWUTILYAML_H
#define KWUTILYAML_H

//#define YAML_BUFFER_SIZE 65536

class KWUtilYaml {
public:

    static int addMapping(yaml_document_t *document, int mapping_node_number, std::string name, std::string value){

        int last_node = (int)(document->nodes.top - document->nodes.start + 1);

        yaml_document_add_scalar(document, (yaml_char_t*)YAML_DEFAULT_SCALAR_TAG, (yaml_char_t*)name.c_str(), (int)name.length(), YAML_PLAIN_SCALAR_STYLE);
        yaml_document_add_scalar(document, (yaml_char_t*)YAML_DEFAULT_SCALAR_TAG, (yaml_char_t*)value.c_str(), (int)value.length(), YAML_PLAIN_SCALAR_STYLE);
        yaml_document_append_mapping_pair(document, mapping_node_number, last_node, last_node+1);

        return 0; // EXIT_SUCCESS
    }

    static int addSequence(yaml_document_t *document, int mapping_node_number, std::string name, std::vector<std::string> vectorOfValues){

        int last_node = (int)(document->nodes.top - document->nodes.start + 1);
        yaml_document_add_sequence(document, (yaml_char_t*)YAML_DEFAULT_SEQUENCE_TAG, YAML_ANY_SEQUENCE_STYLE);
        yaml_document_add_scalar(document, (yaml_char_t*)YAML_DEFAULT_SCALAR_TAG, (yaml_char_t*)name.c_str(), (int)name.length(), YAML_PLAIN_SCALAR_STYLE);

        for (unsigned int i = 0; i < vectorOfValues.size(); i++) {
            yaml_document_add_scalar(document, (yaml_char_t *) YAML_DEFAULT_SCALAR_TAG, (yaml_char_t*)vectorOfValues.at(i).c_str(), (int)vectorOfValues[i].length(), YAML_PLAIN_SCALAR_STYLE);
        }
        yaml_document_append_mapping_pair(document, mapping_node_number, last_node+1, last_node);
        for (unsigned int i = 0; i < vectorOfValues.size(); i++) {
            yaml_document_append_sequence_item(document, last_node, last_node+i+2);
        }

        return 0; // EXIT_SUCCESS
    }

    static void printNodes(const yaml_document_t *document){
        yaml_node_pair_t *pair;
        yaml_node_item_t *item;
        yaml_node_t *node;
        int nn;
        for (node = document->nodes.start; node < document->nodes.top; node ++) {
            nn = node - document->nodes.start + 1;
            switch (node->type) {
                case YAML_SCALAR_NODE:
                    printf("node %d node->type %d node->tag %s %d %s %c\n", nn, node->type, node->tag,
                           (int)node->data.scalar.length, node->data.scalar.value,
                           node->data.scalar.style);
                    break;
                case YAML_MAPPING_NODE:
                    for (pair = node->data.mapping.pairs.start;
                         pair < node->data.mapping.pairs.top; pair++) {
                        printf("node %d node->type %d node->tag %s pair->key %d pair->value %d\n", nn,
                               node->type, node->tag, pair->key, pair->value);
                    }
                    break;
                case YAML_SEQUENCE_NODE:
                    for (item = node->data.sequence.items.start;
                         item < node->data.sequence.items.top; item++) {
                        printf("node %d node->type %d node->tag %s item %d\n", nn, node->type, node->tag,
                               *item);
                    }
                    break;
                default:
                    break;
            }
        }
    }
};

//#undef YAML_BUFFER_SIZE

#endif //KWUTILYAML_H
