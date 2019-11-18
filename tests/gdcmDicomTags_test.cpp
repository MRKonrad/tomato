#include "gtest/gtest.h"

#include "gdcmTomatoReadTags.h"

// TODO: find a sample dataset for testing

TEST(GDCMTags, wrong_file){

    std::string result;

    const std::string filename = "this_is_not_a_real_file.dcm";

    std::vector<std::pair<int, int> > tags;
    tags.push_back(std::pair<int, int> (0x0010, 0x1020));

    int gdcmTomatoReadTagsExit = gdcmTomatoReadTags(tags, filename, result);

    ASSERT_EQ(gdcmTomatoReadTagsExit, 1); // EXIT_FAILURE
    ASSERT_STREQ(result.c_str(), "");

}

TEST(GDCMTags, wrong_tag){

    std::string result;

    const std::string filename = "testData/dicom/Hcmr_Phantom_1916_260C/Konrad_Shmolli/ShMOLLI_192i_e11_12/IM-0001-0001.dcm";

    std::vector<std::pair<int, int> > tags;
    tags.push_back(std::pair<int, int> (0x9999, 0x1021));

    int gdcmTomatoReadTagsExit = gdcmTomatoReadTags(tags, filename, result);

    ASSERT_EQ(gdcmTomatoReadTagsExit, 1); // EXIT_FAILURE
    ASSERT_STREQ(result.c_str(), "");

}

//TEST(GDCMTags, no_nesting){
//
//    std::string result;
//
//    const std::string filename = "/Users/kwerys/Data/fromKostas/T2Map_Vida/raw/CardiacMapping.MR.heart_localizer.13.1.2019.07.15.11.46.48.17.25895139.dcm";
//
//    std::vector<std::pair<int, int> > tags;
//    tags.push_back(std::pair<int, int> (0x0010, 0x1020));
//
//    int gdcmTomatoReadTagsExit = gdcmTomatoReadTags(tags, filename, result);
//
//    ASSERT_EQ(gdcmTomatoReadTagsExit, 0); // EXIT_SUCCESS
//    ASSERT_STREQ(result.c_str(), "1.905 ");
//
//}
//
//TEST(GDCMTags, FrameComments){
//
//    std::string result;
//
//    const std::string filename = "/Users/kwerys/Data/fromKostas/T2Map_Vida/raw/CardiacMapping.MR.heart_localizer.13.1.2019.07.15.11.46.48.17.25895139.dcm";
//    std::vector<std::pair<int, int> > tags;
//    tags.push_back(std::pair<int, int>(0x5200, 0x9230));
//    tags.push_back(std::pair<int, int>(0x0020, 0x9111));
//    tags.push_back(std::pair<int, int>(0x0020, 0x9158));
//
//    int gdcmTomatoReadTagsExit = gdcmTomatoReadTags(tags, filename, result);
//
//    ASSERT_EQ(gdcmTomatoReadTagsExit, 0); // EXIT_SUCCESS
//    ASSERT_STREQ(result.c_str(), "T2 prep. duration = 0 ms");
//}
