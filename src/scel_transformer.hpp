//
// Created by cvrain on 24-3-20.
//

#ifndef SCEL_TRANSFORMER_HPP
#define SCEL_TRANSFORMER_HPP

#include <filesystem>
#include <string>
#include <vector>
#include <optional>

class ScelTransformer {
public:
    explicit ScelTransformer(const std::string &source_path, const std::string &dist_path = "./");

protected:
    void format_dir(const std::string &source_path, const std::string &file_path);
    void format_file(const std::string &source_path, const std::string &file_path);
    static std::string convert_to_absolute_path(const std::string &relative_path);
    static std::string analysis_home_path(const std::string &relative_path);
    static std::vector<std::filesystem::path> traversal_directory(const std::string& dir_path);
    static std::optional<std::string> execute_command(const std::string& command);
    static std::string get_filename_without_extension(const std::string& filename);
    void check_scel2org5_exist();

private:
    const std::string local_scel2org5_path = std::string{getenv("HOME")}.append("/.local/bin/scel2org5");
    const std::string global_scel2org5_path = std::string{"/usr/bin/scel2org5"};
    std::string valid_scel2org5_path{};
};


#endif // SCEL_TRANSFORMER_HPP
