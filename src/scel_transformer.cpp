//
// Created by cvrain on 24-3-20.
//

#include "scel_transformer.hpp"

#include <cassert>
#include <cstdlib>
#include <format>
#include <iostream>

ScelTransformer::ScelTransformer(const std::string &source_path, const std::string &dist_path) {
    // 检查scel2org5是否存在
    check_scel2org5_exist();
    // 将Linux中~目录转换为完整路径
    auto analysis_source_path = analysis_home_path(source_path);
    auto analysis_dist_path = analysis_home_path(dist_path);
    // 将相对路径转换为绝对路径
    analysis_source_path = convert_to_absolute_path(analysis_source_path);
    analysis_dist_path = convert_to_absolute_path(analysis_dist_path);
    if (is_directory(std::filesystem::path(source_path))) {
        format_dir(analysis_source_path, analysis_dist_path);
    }
    else {
        format_file(analysis_source_path, analysis_dist_path);
    }
}
void ScelTransformer::format_dir(const std::string &source_path, const std::string &file_path) {
    const auto source_directory = traversal_directory(source_path);
    if (not is_directory(std::filesystem::path(file_path))) {
        std::cout << "input is a directory and output is a signle file path";
    }
    for (const auto &it: source_directory) {
        const auto filename = get_filename_without_extension(it);
        format_file(it, std::filesystem::path{source_path}.append(filename + ".dict"));
    }
}

void ScelTransformer::format_file(const std::string &source_path, const std::string &file_path) {
    // const std::string command = std::format("{} -t {} -o {}", valid_scel2org5_path, source_path, file_path);
    const std::string command =
            std::vformat("{} -t {} -o {}", std::make_format_args(valid_scel2org5_path, source_path, file_path));
    if (const auto result = execute_command(command); result.has_value()) {
        std::cout << result.value() << std::endl;
    }
}
std::string ScelTransformer::convert_to_absolute_path(const std::string &relative_path) {
    if (not std::filesystem::exists(relative_path)) {
        throw std::runtime_error("This path could not be found");
    }
    if (relative_path.empty()) {
        throw std::runtime_error("File path is empty");
    }

    return std::filesystem::absolute(relative_path);
}

std::string ScelTransformer::analysis_home_path(const std::string &relative_path) {
    auto path = relative_path;
    if (relative_path.at(0) != '~') {
        return relative_path;
    }
    if (relative_path.size() <= 1 or relative_path.at(1) != '/') {
        return relative_path;
    }

    if (const char *home_path = getenv("HOME"); home_path or ((home_path = getenv("USERPROFILE")))) {
        path.replace(0, 1, home_path);
    }
    else {
        char const *hdrive = getenv("HOMEDRIVE"), *hpath = getenv("HOMEPATH");
        assert(hdrive); // or other error handling
        assert(hpath);
        path.replace(0, 1, std::string(hdrive) + hpath);
    }
    return path;
}
std::vector<std::filesystem::path> ScelTransformer::traversal_directory(const std::string &dir_path) {
    std::vector<std::filesystem::path> files;
    for (const auto &entry: std::filesystem::recursive_directory_iterator(dir_path)) {
        if (is_regular_file(entry.path())) {
            files.push_back(entry.path());
        }
    }
    return files;
}
std::optional<std::string> ScelTransformer::execute_command(const std::string &command) {
    char buffer[128];
    std::ostringstream result{};
    FILE *fp = popen(command.data(), "r");
    if (!fp) {
        return std::nullopt;
    }
    while (fgets(buffer, sizeof(buffer), fp)) {
        result << buffer;
    }
    pclose(fp);
    return result.str();
}
std::string ScelTransformer::get_filename_without_extension(const std::string &filename) {
    const size_t dot_pos = filename.rfind('.');
    if (dot_pos == std::string::npos) {
        return filename; // 没有找到后缀名，直接返回原文件名
    }
    return filename.substr(0, dot_pos);
}
void ScelTransformer::check_scel2org5_exist() {
    if (std::filesystem::exists(local_scel2org5_path)) {
        valid_scel2org5_path = local_scel2org5_path;
    }
    else if (std::filesystem::exists(global_scel2org5_path)) {
        valid_scel2org5_path = global_scel2org5_path;
    }
    else {
        throw std::runtime_error("The 'scel2org5' could not be found");
    }
}
