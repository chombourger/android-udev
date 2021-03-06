#  -*- Mode: python; coding: utf-8; indent-tabs-mode: nil -*- */
#
#  This file is part of systemd.
#
#  Copyright 2012-2013 Zbigniew Jędrzejewski-Szmek
#
#  systemd is free software; you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2.1 of the License, or
#  (at your option) any later version.
#
#  systemd is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with systemd; If not, see <http://www.gnu.org/licenses/>.

try:
    from lxml import etree as tree

    class CustomResolver(tree.Resolver):
        def resolve(self, url, id, context):
            if 'custom-entities.ent' in url:
                return self.resolve_filename('man/custom-entities.ent', context)

    _parser = tree.XMLParser()
    _parser.resolvers.add(CustomResolver())
    xml_parse = lambda page: tree.parse(page, _parser)
    xml_print = lambda xml: tree.tostring(xml, pretty_print=True,
                                          encoding='utf-8')
except ImportError:
    import xml.etree.ElementTree as tree
    import re as _re
    import io as _io

    def xml_parse(page):
        s = _re.sub(b'&[a-zA-Z0-9_]+;', b'', open(page, 'rb').read())
        return tree.parse(_io.BytesIO(s))
    xml_print = lambda xml: tree.tostring(xml, encoding='utf-8')
