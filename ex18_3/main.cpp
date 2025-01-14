
#include <gtest/gtest.h>

#include <pcosynchro/pcologger.h>
#include <pcosynchro/pcotest.h>

#include "loader.h"

TEST(ThreadedLoader, standard) {

    ASSERT_DURATION_LE(5, {
                           ThreadedLoader* l = new ThreadedLoader(new
                                   SimpleLoaderFake("../../words.txt"));
                           auto loader = std::unique_ptr<ThreadedLoader>(l);
                           logger() << "starting to load data...";
                           loader->startPreloading();
                           logger() << "waiting on data...";
                           Data* d = loader->getData();
                           auto data = std::unique_ptr<Data>(d);
                           ASSERT_NE(data, nullptr);

                           logger() << "data loaded." << '\n';
                           for (const auto &word : data->words) {
                               logger() << word.toStdString() << '\n';
                           }
                       });
}

TEST(ThreadedLoader, noPreloading) {

    ASSERT_DURATION_LE(5, {
                           ThreadedLoader* l = new ThreadedLoader(new
                                   SimpleLoader("../../words.txt"));
                           auto loader = std::unique_ptr<ThreadedLoader>(l);
                           logger() << "waiting on data...";
                           Data* d = loader->getData();
                           auto data = std::unique_ptr<Data>(d);
                           ASSERT_NE(data, nullptr);
                           logger() << "data loaded." << '\n';
                           for (const auto &word : data->words) {
                               logger() << word.toStdString() << '\n';
                           }
                       });
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    logger().initialize(argc, argv);
    return RUN_ALL_TESTS();
}
