require 'rouge' unless defined? ::Rouge.version

# base: /usr/lib/ruby/vendor_ruby/rouge/themes/pastie.rb
# -or-: /usr/share/rubygems-integration/all/gems/rouge-3.28.0/lib/rouge/themes/pastie.rb

module Rouge; module Themes
  class Custom < Pastie
    name 'custom'

    style Comment,         :fg => '#666666', :italic => true
    style Str,             :fg => '#808000'
    style Str::Escape,     :fg => '#808000'
    style Str::Interpol,   :fg => '#808000'
    style Str::Other,      :fg => '#808000'
    style Name::Constant,  :fg => '#000080'
    style Keyword,         :fg => '#000080', :bold => true
    style Keyword::Type,   :fg => '#80604D', :bold => true
    style Operator::Word,  :fg => '#008800', :bold => true
    style Num,             :fg => '#B400B4'
  end
end; end
